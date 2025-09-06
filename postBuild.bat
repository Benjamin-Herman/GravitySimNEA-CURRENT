if not exist ".\x64\Debug\models" mkdir ".\x64\Debug\models"
if not exist ".\x64\Debug\shaders" mkdir ".\x64\Debug\shaders"
if not exist ".\x64\Debug\saves" mkdir ".\x64\Debug\saves"

xcopy /Y /I ".\models\*.obj" ".\x64\Debug\models\"
xcopy /Y /I ".\shaders\*.vert" ".\x64\Debug\shaders\"
xcopy /Y /I ".\shaders\*.frag" ".\x64\Debug\shaders\"

xcopy /Y /I ".\saves\*.save" ".\x64\Debug\saves\"
