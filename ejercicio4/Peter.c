while (TRUE) 
{
wait(tomarMatePremium)
yerba = tomarDe(frascoPremium);
wait(mutex_despol)
yerbaLimpia = usar(despol,yerba);
signal(mutex_despol)
mate = cargarCon(yerbaLimpia);
disfrutarUnosVerdes(mate);
wait(tomarMatePremium)
signal(llenarFrasco)
}
