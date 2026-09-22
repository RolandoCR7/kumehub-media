# kumehub-media

Gráficas de marca de [Küme Hub](https://kumehub.cl) para publicación en Instagram.

Este repositorio existe por un detalle de la API de Instagram: no acepta que le subas
un archivo, sino que descarga el contenido desde una URL pública. Estas imágenes se
sirven por CDN para que la publicación automatizada pueda tomarlas.

- `h*.jpg` — historias, 1080×1920
- el resto — publicaciones de feed, 1080×1350

Generadas con `src/images.js` del proyecto de automatización, usando las tipografías
(Outfit y DM Sans) y los colores de marca de kumehub.cl.
