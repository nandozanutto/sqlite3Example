SELECT
(select count(distinct(o_custkey)) 
from orders
WHERE o_custkey NOT IN(select o_custkey--sem special request
from orders
where o_comment Like '%special request%')) + --not like nao funcionou n sei pq
(select count(c_custkey) 
from customer
where c_custkey NOT IN(select distinct(o_custkey) from orders));--nao compraram