/* Tiempo.c    Implementaci�n del modulo de tiempo */

#include <Tiempo.h>

/* Rutina para iniciar el m�dulo (su estructura de datos) */
char Tm_Inicie (Tm_Control *tcp,
                Tm_Periodo *pp,
                Tm_Num nper,
                Tm_Timeout *tp,
                Tm_Num nto,
                Tm_Atender *atender)
   {
   /* Tabla de per�odos */
   tcp->pp = pp;
   tcp->nper = nper;
   for (; nper; ++pp, --nper)
      {
      pp->banderas = 0;
      pp->contador = pp->periodo = 0;
      };

   /* Tabla de timeouts/retardos */
   tcp->tp = tp;
   tcp->nto = nto;
   for (; nto; ++tp, --nto)
      *tp = 0;

   /* Rutina para manejar el timer (HW) */
   tcp->atender = atender;

   return SI;
   };

/* Rutina para procesar el m�dulo (dentro del loop de polling) */
void Tm_Procese (Tm_Control *tcp)
   {
   Tm_Num n;
   Tm_Periodo *pp;
   Tm_Timeout *tp;

   if ( !(tcp->atender(SI)) )
      return;

   /* ZONA CRÍTICA */
   for (n = tcp->nper, pp = tcp->pp; n; ++pp, --n)
      if (pp->banderas & TM_PER_F_ACTIVO)
         {
         --(pp->contador);
         if ( !(pp->contador) )
            {
            pp->banderas |= TM_PER_F_FC;
            pp->contador = pp->periodo;
            };
         };
   /* FIN DE ZONA CRÍTICA */

   /* ZONA CRÍTICA */
   for (n = tcp->nto, tp = tcp->tp; n; ++tp, --n)
      if (*tp)
         --(*tp);
   /* FIN DE ZONA CRÍTICA */
   };

/* ===== RUTINAS DE INTERFAZ ====== */
/* Configurar un per�odo para que empiece a funcionar */
char Tm_Inicie_periodo (Tm_Control *tcp,
                        Tm_Num num_periodo,
                        Tm_Contador periodo)
   {
   Tm_Periodo *pp = tcp->pp + num_periodo;

   if (num_periodo >= tcp->nper)
      return NO;

   /* ZONA CRÍTICA */
   pp->banderas = TM_PER_F_ACTIVO;
   pp->contador = pp->periodo = periodo;
   /* FIN DE ZONA CRÍTICA */

   return SI;
   };

/* Desactivar un per�odo para que deje de funcionar */
void Tm_Termine_periodo (Tm_Control *tcp, Tm_Num num_periodo){
   Tm_Periodo *pp = tcp->pp + num_periodo;
   if (num_periodo < tcp->nper)
      /* ZONA CRÍTICA */
      pp -> banderas &= ~(TM_PER_F_ACTIVO);
      /* FIN DE ZONA CRÍTICA */
};

/* Verificar si hubo fin de conteo en un periodo */
char Tm_Hubo_periodo (Tm_Control *tcp, Tm_Num num_periodo){
   Tm_Periodo *pp = tcp->pp + num_periodo;
   if (num_periodo >= tcp->nper)
      return NO;
   if(pp->banderas & TM_PER_F_FC)
      return SI;
   else
      return NO;
};

/* Bajar la bandera de conteo en un periodo */
void Tm_Baje_periodo (Tm_Control *tcp, Tm_Num num_periodo){
   Tm_Periodo *pp = tcp->pp + num_periodo;
   if (num_periodo < tcp->nper)
      /* ZONA CRÍTICA */
      pp -> banderas &= ~(TM_PER_F_FC);
      /* FIN DE ZONA CRÍTICA */
}

/* Configurar un timeout/retardo para que empiece a funcionar */
char Tm_Inicie_timeout (Tm_Control *tcp, Tm_Num num_timeout, Tm_Contador espera)
{
   Tm_Timeout *tp = tcp->tp + num_timeout;

   if (num_timeout >= tcp->nto)
      return NO;

   /* ZONA CRÍTICA */
   *tp = espera;
   /* FIN DE ZONA CRÍTICA */

   return SI;
   };

/* Desactivar un timeout/retardo para que deje de funcionar */
void Tm_Termine_timeout (Tm_Control *tcp, Tm_Num num_timeout){
   Tm_Timeout *tp = tcp->tp + num_timeout;
   if (num_timeout < tcp->nto)
      /* ZONA CRÍTICA */
      *tp = 0;
      /* FIN DE ZONA CRÍTICA */
};

/* Verificar si hubo timeout (o se completo el retardo) */
char Tm_Hubo_timeout (Tm_Control *tcp, Tm_Num num_timeout){
   Tm_Timeout *tp = tcp->tp + num_timeout;
   if (num_timeout < tcp->nto)
      return (char)(!(*tp));
   else
      return NO;
};
