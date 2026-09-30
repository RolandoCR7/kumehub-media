# Rediseño de msclinics.cl

Propuesta de rediseño para [MSClinics](https://msclinics.cl/), distribuidora de insumos médicos
en Puente Alto. Es un solo archivo, `index.html`, sin dependencias ni build: se abre directo en
el navegador. Lo único externo son las tipografías de Google Fonts.

## La idea

El sitio actual es un catálogo donde cada producto termina en "solicitar cotización". La propuesta
convierte esa cotización en el eje de la página: el visitante arma una lista con cantidades y la
envía por WhatsApp con el mensaje ya escrito.

## Qué incluye

- **Hero con monitor de signos vitales.** Un ECG y una curva de pletismografía animados en canvas.
  La franja inferior del monitor muestra la cotización en curso, y cada producto agregado dispara
  un latido.
- **Catálogo filtrable.** Categorías, filtro "para uso en casa" y búsqueda sin tildes. Las tarjetas
  se reordenan con la View Transitions API.
- **Cotización persistente.** La lista se guarda en `localStorage`, genera el mensaje de WhatsApp
  con cada producto y su cantidad, y tiene vista previa, "copiar texto" y "vaciar" con confirmación.
- **Equipa tu espacio.** Listas base para box de atención, toma de muestras, cuidado en casa y
  urgencia. Se ajustan y se agregan completas a la cotización.
- **Búsqueda rápida** con `Ctrl K` / `⌘ K` o `/`: productos y secciones, navegable con teclado.
  `Shift + Enter` agrega el producto sin abrirlo.
- **Detalle de producto** con enlace a la ficha técnica PDF cuando existe (catre clínico, camilla
  de traslado y monitor M-3T usan los PDF publicados hoy en msclinics.cl).
- **Sección institucional**, proceso de cotización, la donación a Honduras de diciembre de 2024,
  preguntas frecuentes y contacto con formulario que abre WhatsApp.
- **Tema claro y oscuro** (sigue al sistema, con botón que recuerda la elección).

### Técnicas

- CSS: scroll-driven animations (`animation-timeline: view()` y barra de progreso con `scroll()`),
  container queries en el monitor, `@starting-style` para abrir diálogos, `color-mix()`.
- `<dialog>` nativo para la cotización, el detalle y la búsqueda (foco atrapado y `Esc` gratis).
- Accesibilidad: enlace para saltar al catálogo, foco visible, `aria-live` en avisos, pestañas con
  flechas del teclado, y `prefers-reduced-motion` respetado (el monitor queda estático).
- SEO: datos estructurados `Store` en JSON-LD con dirección y teléfonos, y etiquetas Open Graph.

## Contenido

Los nombres de producto, la dirección, los teléfonos, la misión y la nota de Honduras salen de
msclinics.cl. Las descripciones de producto se redactaron para la propuesta y no incluyen
especificaciones técnicas: hay que validarlas con el cliente. Algunos productos (camilla de examen,
guantes, alcohol gel, andador) se infirieron de las categorías del sitio y hay que confirmarlos.

Las ilustraciones de producto son SVG lineales, dibujadas para que el catálogo se vea consistente
mientras no haya fotos propias.

## Para pasar a producción

1. Conectar el catálogo a la tienda actual. El sitio corre en WordPress con WooCommerce (según sus
   URLs), así que `PRODUCTS` puede llenarse desde la API REST de WooCommerce en vez de estar escrito
   en el HTML.
2. Reemplazar las ilustraciones por fotos reales de producto.
3. Confirmar con el cliente: correo, horario de atención, si hay retiro en Puente Alto y a qué
   comunas despachan.
4. Quitar `<meta name="robots" content="noindex">`.
5. Medir los envíos de cotización (clic en "Enviar cotización por WhatsApp") como conversión.
