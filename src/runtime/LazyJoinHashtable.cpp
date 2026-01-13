#include "lingodb/runtime/LazyJoinHashtable.h"
#include "lingodb/runtime/GrowingBuffer.h"
#include "lingodb/utility/Tracer.h"

#include <algorithm>
#include <atomic>
#include <iostream>

namespace {
static lingodb::utility::Tracer::Event buildEvent("HashIndexedView", "build");
} // end namespace
lingodb::runtime::HashIndexedView* lingodb::runtime::HashIndexedView::build(lingodb::runtime::GrowingBuffer* buffer) {
   utility::Tracer::Trace trace(buildEvent);
   auto* executionContext = runtime::getCurrentExecutionContext();
   auto& values = buffer->getValues();
   size_t htSize = std::max(nextPow2(values.getLen() * 1.25), static_cast<uint64_t>(1));
   size_t htMask = htSize - 1;
   auto* htView = new HashIndexedView(htSize, htMask);
   //std::cerr << "BUILD: " << (htView) << "\n";
   values.iterateParallel([&](uint8_t* ptr) {
      auto* entry = (Entry*) ptr;

      size_t hash = (size_t) entry->hashValue;
      auto pos = hash & htMask;
      std::atomic_ref<Entry*> slot(htView->ht[pos]);
      Entry* current = slot.load();
      Entry* newEntry;
      do {
         entry->next = lingodb::runtime::untag(current);
         newEntry = lingodb::runtime::tag(entry, current, hash);
      } while (!slot.compare_exchange_weak(current, newEntry));
   });
   //std::cerr << "Finish " << htView << "\n";
   // Mark view as finished and register for cleanup only after the build completed.
   htView->finished.store(true);
   executionContext->registerState({htView, [](void* ptr) { delete reinterpret_cast<lingodb::runtime::HashIndexedView*>(ptr); }});
   trace.stop();
   return htView;
}
void lingodb::runtime::HashIndexedView::destroy(lingodb::runtime::HashIndexedView* ht) {
   delete ht;
}
lingodb::runtime::HashIndexedView::HashIndexedView(size_t htSize, size_t htMask) : ht(lingodb::runtime::FixedSizedBuffer<Entry*>::createZeroed(htSize)), htMask(htMask) {}
lingodb::runtime::HashIndexedView::~HashIndexedView() {
   lingodb::runtime::FixedSizedBuffer<Entry*>::deallocate(ht, htMask + 1);
}
