# Cinder-WMFMovieWriter
Simple movie recorder for Cinder using the Windows Media foundation.

This clocks was made by hacking [Tutorial: Using the Sink Writer to Encode Video](https://msdn.microsoft.com/en-us/library/windows/desktop/ff819477(v=vs.85).aspx). IT is not meant to be seen as a professional library nor will I be giving any kind of support.

For now it just writes h264 and WMV3 files, but you can modified it to suit your needs.


The class has two different method for adding frames, ones expects a Surface a s a parameter and the other a reference to a Texture.
```cpp
void addFrame( Surface imageSource, float duration = -1.0f);
void addFrame( gl::TextureRef textureSource, float duration = -1.0f);
```			
The WMF expects a unsigned char * array, so a surface is the perfect choice but if the data of a surface is passed, the color red a blue are swaped. 
The fastest solution found was to render to a FBO and swap the color in a shader.





