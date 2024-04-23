# tp1-sistemas-operativos
tp1 sistemas operativos

Enunciado:
Manucho que cursó en el segundo cuatrimestre de 2023 la materia Sistemas Operativos y
Redes, decide hacer un gran asado en su casa para festejar que promocionó la materia. Como
además Manucho hace poco ganó el Loto plus y no escatima en gastos y decide contratar un servicio
de catering.
El servicio consiste de 1 asador y M mozos.
A la casa de Mancucho asistirán N invitados. Los comensales se sientan a la mesa, siendo Manucho
el último en sentarse.
Una vez que se sienta a la mesa Manucho, los M mozos sirven la comida. Por problemas de
organización de la empresa de catering, la cantidad de mozos es menor a la cantidad de invitados.
Los mozos no deben servir más de N+1 platos. Cualquier comensal incluido nuestro querido
Manucho puede comenzar a comer, incluso si no han terminado de servir a todos los invitados.
A Manucho le encanta hablar sobre futbol en especial sobre el mundial. Cuando Manucho termina
de comer hace una pregunta sobre los favoritos a ser campeones del mundo. Esta pregunta puede
ser respondida por cualquier invitado/a que haya terminado de comer. Solo un invitado debe
responder la pregunta.
Como es de esperarse en un asado, habrá vino así que los ánimos sobre cuestiones de futbol se
exacerban y Manucho se enoja muchísimo por la respuesta un invitado/a se levanta y abandona el
quincho. Si Manucho ya se ha levantado, los invitados pueden hacer lo mismo, pero solo si ya han
terminado de comer.
Las primitivas disponibles son:
Sentarse()
Servircomida()
Enojarse()
Levantarse()
Lanzar_pregunta_mundialista()
Lanzar_respuesta_mundialista()
Consignas:
1. Realizar una solución usando semáforos para el problema descripto
2. Explicar brevemente cómo funciona la solución indicando que semáforos necesitó y para
qué; y cuantos procesos diferentes tuvo.
3. ¿En algún punto de la solución se pude producir inanición? ¿Dónde? ¿Por qué?
4. Implementar la solución en lenguaje C utilizando semáforos y threads
