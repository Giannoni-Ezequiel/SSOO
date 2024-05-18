while(true){
    wait(pedidoTomado)
    wait(parrillaLibre)
    wait(muchosPatys)
    cocinarPaty()
    --Paty hecha
    signal(parrillaLibre)
    signal(patyCocido)
}
