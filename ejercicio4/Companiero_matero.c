while (TRUE) 
{
wait(tomarMateNormal)
yerba = tomarDe(frasco);
wait(mutex_despol)
yerbaLimpia = usar(despol,yerba);
signal(mutex_despol)
mate =cargarCon(yerbaLimpia);
saborearUnosVerdes(mate);
signal(llenarFrasco)
}
