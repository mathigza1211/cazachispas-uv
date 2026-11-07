# Cazachispas UV: detector de arcos eléctricos por fotones ultravioleta

Prototipo de bajo costo (≈ S/ 180 con materiales de demo; núcleo ≈ S/ 70) que detecta la chispa de un arco eléctrico por su luz UV (337 nm, segundo sistema positivo del N₂) y confirma con el ruido de la corriente; si ambos coinciden, un relé corta la energía. Proyecto escolar de Innova Schools para "El Gran Reto" (circuitos eléctricos: uso eficiente y seguro de la electricidad). Lema: *Vemos la chispa antes que el fuego.*

> **Estado:** diseño, código y simulador. **Las pruebas de la sección 9 del informe (tabla de aciertos y tiempos) aún no tienen resultados medidos**; no se afirma una tasa de aciertos ni un tiempo de corte real.

## Cómo usarlo
- **Simulador:** abre `index.html` en el navegador. Provoca fallas (empalme flojo, chispa, encendedor de arco), ajusta umbrales y mira cómo decide el Arduino. Incluye un Serial Plotter simulado y un panel de física (energía del fotón vs. brecha del GaN, 3.4 eV).
- **Código:** `cazachispas_uv.ino` (Arduino Uno/Nano), transcrito del informe; calibrar `UMBRAL_UV` y `UMBRAL_RUIDO` con el prototipo real.

## Qué hace la lógica
1. Línea base de UV que se adapta lentamente (sol).
2. Pulsos UV rápidos sobre línea base + umbral.
3. Corte si hay pulso UV **y** corriente inestable, o ≥ 3 pulsos UV en 2 s. Rearme manual.

## Límites (importante)
- Es un prototipo de 12 V: no reemplaza un AFCI certificado ni debe probarse en 220 V (solo un electricista certificado).
- El sensor necesita línea de vista; el módulo UV responde en ≈ 0.5 s, por lo que una chispa única muy corta puede pasar desapercibida.
- La luz UV del sol puede acercarse al umbral: hay que calibrar en el lugar real.
- Los valores del simulador son ilustrativos; el simulador reproduce la lógica del código, no mediciones.

## Documentación
`Cazachispas_UV_Informe_tecnologico.pdf` (problema, física con QED, diseño, presupuesto, pruebas, seguridad, referencias).

## Autoría
Mathias Zárate Amenero, Lima, Perú (proyecto escolar de Innova Schools). Desarrollado con asistencia de Claude (Anthropic).
