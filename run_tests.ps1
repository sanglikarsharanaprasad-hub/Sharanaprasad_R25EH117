# run_tests.ps1
# Script to compile and test graphics_editor.exe

# Compile if not present
if (-not (Test-Path ".\graphics_editor.exe")) {
    Write-Host "Compiling graphics_editor.exe..." -ForegroundColor Cyan
    gcc -O2 -Wall -o graphics_editor.exe main.c
    if (-not (Test-Path ".\graphics_editor.exe")) {
        Write-Error "Failed to compile graphics_editor.exe. Make sure gcc is installed."
        Exit 1
    }
}

$TotalTests = 0
$PassedTests = 0

function Run-Test {
    param(
        [string]$Name,
        [string]$InputSequence,
        [string]$ExpectedPattern
    )
    $script:TotalTests++
    Write-Host "Running: $Name... " -NoNewline -ForegroundColor Yellow

    # Run the editor with input piped
    $output = $InputSequence | .\graphics_editor.exe

    if ($output -match $ExpectedPattern) {
        Write-Host "PASS" -ForegroundColor Green
        $script:PassedTests++
    } else {
        Write-Host "FAIL" -ForegroundColor Red
        Write-Host "Expected output to match pattern: $ExpectedPattern" -ForegroundColor DarkGray
        Write-Host "Actual output sample:" -ForegroundColor DarkGray
        Write-Host ($output | Out-String) -ForegroundColor DarkGray
    }
}

Write-Host "=============================" -ForegroundColor Magenta
Write-Host "   RUNNING GRAPHICS EDITOR TESTS" -ForegroundColor Magenta
Write-Host "=============================" -ForegroundColor Magenta

# Test 1: Add Line and verify rendering and list
# Inputs: 1 (Add), 1 (Line), 5 (x1), 5 (y1), 15 (x2), 10 (y2), 4 (List), [Enter], 5 (Exit)
Run-Test `
    -Name "Add Line" `
    -InputSequence "1`n1`n5`n5`n15`n10`n4`n`n5" `
    -ExpectedPattern "Line from \(5, 5\) to \(15, 10\)"

# Test 2: Add Circle and verify rendering and list
# Inputs: 1 (Add), 3 (Circle), 25 (cx), 10 (cy), 7 (r), 4 (List), [Enter], 5 (Exit)
Run-Test `
    -Name "Add Circle" `
    -InputSequence "1`n3`n25`n10`n7`n4`n`n5" `
    -ExpectedPattern "Circle: Center \(25, 10\), Radius 7"

# Test 3: Add Rectangle and verify rendering and list
# Inputs: 1 (Add), 2 (Rectangle), 30 (x1), 2 (y1), 45 (x2), 8 (y2), 4 (List), [Enter], 5 (Exit)
Run-Test `
    -Name "Add Rectangle" `
    -InputSequence "1`n2`n30`n2`n45`n8`n4`n`n5" `
    -ExpectedPattern "Rectangle: Top-Left \(30, 2\), Bottom-Right \(45, 8\)"

# Test 4: Add Triangle and verify rendering and list
# Inputs: 1 (Add), 4 (Triangle), 10 (x1), 2 (y1), 2 (x2), 8 (y2), 18 (x3), 8 (y3), 4 (List), [Enter], 5 (Exit)
Run-Test `
    -Name "Add Triangle" `
    -InputSequence "1`n4`n10`n2`n2`n8`n18`n8`n4`n`n5" `
    -ExpectedPattern "Triangle: P1\(10, 2\), P2\(2, 8\), P3\(18, 8\)"

# Test 5: Delete Shape
# Inputs: 1 (Add), 3 (Circle), 10 (cx), 10 (cy), 5 (r), 3 (Delete), 1 (ID), 4 (List), [Enter], 5 (Exit)
Run-Test `
    -Name "Delete Shape" `
    -InputSequence "1`n3`n10`n10`n5`n3`n1`n4`n`n5" `
    -ExpectedPattern "\(No objects currently added\.\)"

# Test 6: Modify Shape
# Inputs: 1 (Add), 3 (Circle), 10 (cx), 10 (cy), 5 (r), 2 (Modify), 1 (ID), 20 (cx), 10 (cy), 5 (r), 4 (List), [Enter], 5 (Exit)
Run-Test `
    -Name "Modify Shape" `
    -InputSequence "1`n3`n10`n10`n5`n2`n1`n20`n10`n5`n4`n`n5" `
    -ExpectedPattern "Circle: Center \(20, 10\), Radius 5"

# Test 7: Input Validation on coordinates
# Inputs: 1 (Add), 1 (Line), -5 (invalid), 5 (valid x1), 5 (y1), 15 (x2), 10 (y2), 5 (Exit)
Run-Test `
    -Name "Input Validation (Boundary Check)" `
    -InputSequence "1`n1`n-5`n5`n5`n15`n10`n5" `
    -ExpectedPattern "Value out of range \[0, 49\]\. Please try again\."

Write-Host "=============================" -ForegroundColor Magenta
$ResultColor = "Red"
if ($PassedTests -eq $TotalTests) { $ResultColor = "Green" }
Write-Host "Test Results: $PassedTests / $TotalTests Passed" -ForegroundColor $ResultColor
Write-Host "=============================" -ForegroundColor Magenta

if ($PassedTests -ne $TotalTests) {
    Exit 1
}
