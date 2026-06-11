# Test Cases for 2D Graphics Editor

This document outlines the test suite designed to verify the drawing algorithms, CRUD operations, boundary constraints, and user input validation of the 2D Graphics Editor.

## Automated Testing

An automated PowerShell test script is provided in [run_tests.ps1](file:///C:/Users/sangl/.gemini/antigravity/scratch/Sharanaprasad_R25EH117/run_tests.ps1). It executes the compiled executable with various piped inputs and asserts that shapes are correctly rendered or modified.

### To Run the Automated Tests:
Open PowerShell in the project root and execute:
```powershell
.\run_tests.ps1
```

---

## Test Scenarios

### 1. Test Case: Draw Line (Bresenham's Line Algorithm)
* **Objective**: Ensure that a line is drawn accurately between two coordinates using asterisks (`*`).
* **Input Sequence**:
  - Add Object -> Line -> Start X: `5`, Start Y: `5`, End X: `15`, End Y: `10`
* **Expected Result**:
  - Canvas displays asterisks along the diagonal from `(5,5)` to `(15,10)`.
  - Listing objects displays: `Line from (5, 5) to (15, 10)`.

### 2. Test Case: Draw Circle (Bresenham's Midpoint Circle Algorithm)
* **Objective**: Ensure a symmetric circle is plotted correctly.
* **Input Sequence**:
  - Add Object -> Circle -> Center X: `25`, Center Y: `10`, Radius: `7`
* **Expected Result**:
  - A symmetrical circle drawn using `*` centered at `(25,10)` with radius `7`.
  - Listing objects displays: `Circle: Center (25, 10), Radius 7`.

### 3. Test Case: Draw Rectangle
* **Objective**: Ensure a rectangle is drawn with straight horizontal and vertical borders.
* **Input Sequence**:
  - Add Object -> Rectangle -> Top-Left X: `30`, Top-Left Y: `2`, Bottom-Right X: `45`, Bottom-Right Y: `8`
* **Expected Result**:
  - A closed box shape drawn on the canvas from columns `30` to `45` and rows `2` to `8`.
  - Listing objects displays: `Rectangle: Top-Left (30, 2), Bottom-Right (45, 8)`.

### 4. Test Case: Draw Triangle
* **Objective**: Ensure a triangle is drawn correctly connecting three distinct vertices.
* **Input Sequence**:
  - Add Object -> Triangle -> P1: `(10, 2)`, P2: `(2, 8)`, P3: `(18, 8)`
* **Expected Result**:
  - Three intersecting lines forming a triangle.
  - Listing objects displays: `Triangle: P1(10, 2), P2(2, 8), P3(18, 8)`.

### 5. Test Case: Modify Shape
* **Objective**: Verify that modifying an existing object updates its parameters and redraws the canvas.
* **Input Sequence**:
  - Add Object -> Circle -> Center X: `10`, Center Y: `10`, Radius: `5`
  - Modify Object -> ID: `1` -> New Center X: `20`, New Center Y: `10`, Radius: `5`
* **Expected Result**:
  - The circle is redrawn at its new center `(20, 10)`.
  - Old coordinates at center `(10, 10)` are cleared back to `_`.

### 6. Test Case: Delete Shape
* **Objective**: Verify that deleting an object removes it from the list and clears it from the canvas.
* **Input Sequence**:
  - Add Object -> Rectangle -> Top-Left X: `30`, Top-Left Y: `2`, Bottom-Right X: `40`, Bottom-Right Y: `8`
  - Delete Object -> ID: `1`
* **Expected Result**:
  - The canvas returns to an empty grid (`_` only).
  - Listing active objects displays `(No objects currently added.)`.

### 7. Test Case: Input Validation (Boundary Checks)
* **Objective**: Confirm the program rejects out-of-bounds inputs and non-numeric inputs.
* **Input Sequence**:
  - Add Object -> Line -> Start X: `-5` (expected rejection) -> Start X: `55` (expected rejection) -> Start X: `abc` (expected rejection) -> Start X: `10` (valid)
* **Expected Result**:
  - Rejection messages prompt the user to input a valid value in the range `[0, 49]`.
  - The application remains stable and does not enter an infinite loop.
