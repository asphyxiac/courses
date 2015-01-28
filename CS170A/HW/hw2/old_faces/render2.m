function render2(f)
   imshow(reshape(uint8(abs(f)/max(abs(f)) * 255),64,64))