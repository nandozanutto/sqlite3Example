select c_mktsegment, count(c_mktsegment)
from customer
group by c_mktsegment;