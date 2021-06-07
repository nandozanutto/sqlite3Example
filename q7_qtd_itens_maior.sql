select nation.n_name, SUM(l_quantity)
from nation
inner join customer on nation.n_nationkey = customer.c_nationkey
inner join orders on orders.o_custkey = customer.c_custkey
inner join lineitem on lineitem.l_orderkey = orders.o_orderkey
group by nation.n_name
having (SUM(l_quantity)) > 62000
order by SUM(l_quantity) DESC;