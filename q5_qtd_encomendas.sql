select nation.n_name, count(o_orderkey)
from nation
inner join customer on nation.n_nationkey = customer.c_nationkey
inner join orders on orders.o_custkey = customer.c_custkey
group by nation.n_name
order by count(o_orderkey) DESC;