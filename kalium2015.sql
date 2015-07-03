-- write your code in SQLite 3.8.6
create table noncont (a integer not null, b integer not null );
insert into noncont
select * 
from segments s_out
where not exists (select * from segments s_in
				where s_out.l>=s_in.l
				and s_out.r<=s_in.r
				and (s_out.r<>s_in.r or s_out.l<>s_in.l)
				)order by s_out.l asc;

select (select IFNULL(sum(b-a),0) from noncont) + (select IFNULL(sum(t2.a-t1.b),0) from noncont t1,noncont t2
			where t1.a<=t2.a and (t1.b>=t2.a) and (t1.a<>t2.a or t1.b<>t2.b)
			and t2.a = (select min(t3.a) from noncont t3
            where t3.a>=t1.a and (t3.a<>t1.a or t3.b<>t1.b))
		);