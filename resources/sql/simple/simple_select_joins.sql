select s.matrnr, s.name, v.titel, p.persnr, p.name
from studenten as s
         join hoeren as h on h.matrnr=s.matrnr
         join vorlesungen as v on h.vorlnr=v.vorlnr
         join professoren as p on p.persnr=v.gelesenVon
where s.semester>=4 and s.semester>=3
