@echo off

REM Сохранить текущую директорию
set "WORK_DIR=%CD%"

REM Проверка на существование файла ярлыка и его удаление
IF EXIST "%WORK_DIR%\plot_graph.lnk" (
    del "%WORK_DIR%\plot_graph.lnk"
)

REM Перейти в plotting для создания ярлыка
cd plotting

REM Создание ярлыка через PowerShell и установка рабочей папки
powershell -command "$s=(New-Object -COM WScript.Shell).CreateShortcut((Get-Location).Path + '\dist\plot_graph.lnk'); $s.TargetPath=(Get-Location).Path + '\dist\main.exe'; $s.WorkingDirectory='%WORK_DIR%\plotting\dist'; $s.Save()"

REM Переместить ярлык в исходную директорию
move "dist\plot_graph.lnk" "%WORK_DIR%"