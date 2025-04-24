select s.matrnr, avg(v.sws)
from studenten as s
         join hoeren as h on h.matrnr=s.matrnr
         join vorlesungen as v on h.vorlnr=v.vorlnr
where s.semester>=0 and s.semester>=1
group by s.matrnr
having v.sws > 2