#include "build/temp/_test_puerta.c"
#include "cola_eventos.h"
#include "puerta.h"
#include "unity.h"


puerta_t puerta;



void setUp(void) {

    limpiar_eventos();

    puerta = PuertaCrear(registrar_evento);

}



void tearDown(void) {



}



void enviar_eventos(evento_t * eventos, uint32_t cantidad) {

    int indice;



    for(indice = 0; indice < cantidad / sizeof(evento_t); indice++) {

        PuertaNotificar(puerta, &eventos[indice]);

    }

}



void test_puerta_forzada(void) {

    evento_t eventos[] = {

        { EVENTO_ENTRADA_ACTIVA, PUERTA_SENSOR_ABIERTA }

    };





    enviar_eventos(eventos, sizeof(eventos));





    if ((buscar_evento(EVENTO_ENVIAR_MENSAJE, PUERTA_FORZADA) >= 0)) {} else {UnityFail( ((" Expression Evaluated To FALSE")), (UNITY_UINT)((UNITY_UINT)(33)));};

}
