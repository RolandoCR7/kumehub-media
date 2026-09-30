# Rediseño de msclinics.cl

Propuesta de rediseño para [MSClinics](https://msclinics.cl/), distribuidora de insumos médicos
en Puente Alto. Es un solo archivo, `index.html`, sin librerías ni build: se abre directo en el
navegador. Lo único externo son las tipografías de Google Fonts (Archivo, Geist y Geist Mono).

## La idea

La página entera funciona como un monitor de signos vitales de noche. Cada categoría es un canal
con el color de su curva (ECG, SpO₂, PNI, TEMP, CO₂) y el trazo ECG del hero marca el pulso de
todo el sitio. La cotización por WhatsApp es el eje: el visitante arma una lista con cantidades y
la envía con el mensaje ya escrito.

## Qué incluye

- **Arranque tipo monitor**: autodiagnóstico de un segundo, una vez por sesión. Se salta con un clic
  o una tecla y no aparece si el usuario pidió reducir el movimiento.
- **Hero en vivo**:
  - Fondo WebGL (shader propio) que reacciona al mouse.
  - Trazo ECG y pletismografía dibujados en canvas.
  - Tarjetas FC, SpO₂ y PNI, y el estado de la cotización.
  - En cada latido la palabra "latido" se ensancha con el eje de ancho variable de Archivo.
  - Cada producto agregado dispara un latido extra.
- **Cabecera** con una línea ECG que se dibuja a medida que se recorre la página.
- **Categorías en bento**: cada tarjeta muestra la curva animada de su canal, con foco de luz que
  sigue al cursor e inclinación 3D.
- **Catálogo** con filtros por canal, "uso en casa" y búsqueda sin tildes. Las tarjetas se reordenan
  con la View Transitions API.
- **Cómo cotizar**: historia con scroll fijo. Los pasos avanzan mientras un teléfono muestra la
  conversación de ejemplo por WhatsApp.
- **Equipa tu espacio**: listas base para cuatro tipos de espacio. Los productos orbitan alrededor
  del espacio elegido y la lista se agrega completa a la cotización.
- **Fichas técnicas** como hojas de papel en abanico que se abren al pasar el cursor. Enlazan a los
  PDF reales publicados hoy en msclinics.cl.
- **Compromiso**: ruta animada Santiago → Honduras (SMIL + trazo SVG) con la guía del envío.
- **Búsqueda rápida** con `Ctrl K` / `⌘ K` o `/`: productos y secciones, navegable con teclado.
  `Shift + Enter` agrega el producto sin abrirlo.
- **Cierre** con la marca a gran tamaño, recorrida por un brillo que sigue al scroll.
- Cursor propio, botones magnéticos y texto que se decodifica al aparecer (solo con mouse y
  movimiento permitido).

### Técnicas

- WebGL con shader de ruido fractal propio, sin Three.js.
- Canvas 2D para el trazo del monitor, con barrido y latidos generados en tiempo real.
- CSS: fuente variable animada (`font-stretch`), `animation-timeline: view()`, container queries
  (unidades `cqi` en la órbita), `@starting-style` para abrir diálogos, `color-mix()` para los
  colores de cada canal y `backdrop-filter`.
- `<dialog>` nativo para la cotización, el detalle y la búsqueda.
- Accesibilidad: enlace para saltar al catálogo, foco visible, `aria-live` en avisos, pestañas con
  flechas del teclado y `prefers-reduced-motion` respetado en todo (el monitor queda estático, sin
  arranque ni cursor).
- Rendimiento: el hero se pausa cuando no está en pantalla o la pestaña está oculta, y el shader
  se dibuja a 55 % de resolución.
- SEO: datos estructurados `Store` en JSON-LD con dirección y teléfonos, y etiquetas Open Graph.

## Contenido

Los nombres de producto, la dirección, los teléfonos, la misión y la nota de Honduras salen de
msclinics.cl. Las descripciones de producto se redactaron para la propuesta y no incluyen
especificaciones técnicas: hay que validarlas con el cliente. Algunos productos (camilla de examen,
guantes, alcohol gel, andador) se infirieron de las categorías del sitio y hay que confirmarlos.

Las cifras del monitor del hero (72 lpm, 98 %, 118/76) son decorativas. La conversación de
WhatsApp de la sección "Cómo cotizar" está marcada como ejemplo.

Las ilustraciones de producto son SVG lineales dibujados para la propuesta, en el color del canal
de cada categoría, mientras no haya fotos propias.

## Para pasar a producción

1. Conectar el catálogo a la tienda actual. El sitio corre en WordPress con WooCommerce (según sus
   URLs), así que `PRODUCTS` puede llenarse desde la API REST de WooCommerce en vez de estar escrito
   en el HTML.
2. Reemplazar las ilustraciones por fotos reales de producto, recortadas sobre fondo oscuro.
3. Confirmar con el cliente: correo, horario de atención, si hay retiro en Puente Alto y a qué
   comunas despachan.
4. Quitar `<meta name="robots" content="noindex">`.
5. Medir los envíos de cotización (clic en "Enviar cotización por WhatsApp") como conversión.
