while(true){
    wait(patyCocido) 
    wait(mutexPedido)
    prepararPedido()
    signal(mutexPedido)
    signal(muchosPatys)
    signal(pedidoArmado)

}