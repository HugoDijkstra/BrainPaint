# BrainPaint

Brainpaint is a version of  brainf*ck for generating images.
---
## Syntax:

 >* +: add 1 to the value of the current byte
 >* -: subtract 1 from the current byte
 >* Right arrow: move to the next byte
 >* <: move to the last byte
 >* [: start a loop
 >* ]: end a loop, if the value the of the current byte is 0 then it goes the the next byte, else it will move the the start of the loop
 >* $: take the next 5 bytes and use the to draw a pixel, example bytes:5,5,255,255,255. will draw a white pixel at the 5th row from the top and the side
 >* .: print something in the console
