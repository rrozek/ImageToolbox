@ECHO OFF

setx MAGICK_CODER_MODULE_PATH "%cd%\magick\modules\coders"
setx MAGICK_FILTER_MODULE_PATH "%cd%\magick\modules\filters"
setx MAGICK_CONFIGURE_PATH "%cd%\magick\configuration"

testApp.exe "D:\repos\PBS_Software\tifImages"