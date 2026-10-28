$cmake = "C:\Qt\Tools\CMake_64\bin\cmake.exe"
$qtDir = "C:\Qt\6.12.0\mingw_64"
$ninjaDir = "C:\Qt\Tools\Ninja"
$mingwBin = "C:\Qt\Tools\mingw1310_64\bin"

if (!(Test-Path $mingwBin)) {
    $mingwBin = (Get-ChildItem -Path "C:\Qt\Tools" -Filter "*mingw*bin" -Recurse | Select-Object -First 1 -ExpandProperty FullName)
}

$env:PATH = "$qtDir\bin;$ninjaDir;$mingwBin;" + $env:PATH

Set-Location "C:\Users\bocon\Desktop\os_project"
Write-Host "Configuring CMake with Ninja..."
& $cmake -B build -G Ninja -DCMAKE_PREFIX_PATH="$qtDir"
Write-Host "Building target..."
& $cmake --build build

