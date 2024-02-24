# Establecer la política de ejecución (si es necesario)
# Set-ExecutionPolicy RemoteSigned

for ($j = 1; $j -le 10; $j++) {
    foreach ($i in 10, 100, 200, 400, 600, 800) {
        .\a.exe $i | Out-File -Append -FilePath resultados-windows.txt
    }
}

# Restaurar la política de ejecución a la configuración original (si es necesario)
# Set-ExecutionPolicy Restricted
