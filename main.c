#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 20
#define COLS 50
#define MAX_OBJECTS 100

typedef enum {
    SHAPE_LINE = 1,
    SHAPE_RECTANGLE,
    SHAPE_CIRCLE,
    SHAPE_TRIANGLE
} ShapeType;

typedef struct {
    int x1, y1;
    int x2, y2;
} LineData;

typedef struct {
    int x1, y1;
    int x2, y2;
} RectData;

typedef struct {
    int cx, cy;
    int r;
} CircleData;

typedef struct {
    int x1, y1;
    int x2, y2;
    int x3, y3;
} TriangleData;

typedef struct {
    int id;
    ShapeType type;
    union {
        LineData line;
        RectData rect;
        CircleData circle;
        TriangleData triangle;
    } data;
} GraphicObject;

GraphicObject objects[MAX_OBJECTS];
int objectCount = 0;
int nextId = 1;
char canvas[ROWS][COLS];

// Helper functions for user input validation
int getIntInput(const char *prompt, int minVal, int maxVal) {
    int val;
    char term;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d%c", &val, &term) != 2 || term != '\n') {
            // Clear input buffer
            while (getchar() != '\n');
            printf("Invalid input. Please enter an integer.\n");
        } else if (val < minVal || val > maxVal) {
            printf("Value out of range [%d, %d]. Please try again.\n", minVal, maxVal);
        } else {
            return val;
        }
    }
}

// Drawing helper to safely plot a point on the canvas
void plotPoint(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas[y][x] = '*';
    }
}

// Bresenham's Line Algorithm
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        plotPoint(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Rectangle Drawing (composed of 4 lines)
void drawRectangle(int x1, int y1, int x2, int y2) {
    drawLine(x1, y1, x2, y1); // Top
    drawLine(x1, y2, x2, y2); // Bottom
    drawLine(x1, y1, x1, y2); // Left
    drawLine(x2, y1, x2, y2); // Right
}

// Bresenham's Circle Algorithm
void drawCircle(int cx, int cy, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y) {
        plotPoint(cx + x, cy + y);
        plotPoint(cx - x, cy + y);
        plotPoint(cx + x, cy - y);
        plotPoint(cx - x, cy - y);
        plotPoint(cx + y, cy + x);
        plotPoint(cx - y, cy + x);
        plotPoint(cx + y, cy - x);
        plotPoint(cx - y, cy - x);

        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// Triangle Drawing (composed of 3 lines)
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

// Renders all current shapes onto the canvas
void renderCanvas() {
    // Initialize canvas with '_'
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            canvas[r][c] = '_';
        }
    }

    // Render each graphic object
    for (int i = 0; i < objectCount; i++) {
        GraphicObject obj = objects[i];
        switch (obj.type) {
            case SHAPE_LINE:
                drawLine(obj.data.line.x1, obj.data.line.y1, obj.data.line.x2, obj.data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                drawRectangle(obj.data.rect.x1, obj.data.rect.y1, obj.data.rect.x2, obj.data.rect.y2);
                break;
            case SHAPE_CIRCLE:
                drawCircle(obj.data.circle.cx, obj.data.circle.cy, obj.data.circle.r);
                break;
            case SHAPE_TRIANGLE:
                drawTriangle(obj.data.triangle.x1, obj.data.triangle.y1, obj.data.triangle.x2, obj.data.triangle.y2, obj.data.triangle.x3, obj.data.triangle.y3);
                break;
        }
    }
}

// Displays canvas with grid headers
void displayCanvas() {
    printf("\nCanvas size: %d rows x %d cols\n", ROWS, COLS);
    printf("   ");
    for (int c = 0; c < COLS; c++) {
        printf("%d", c % 10);
    }
    printf("\n");
    for (int r = 0; r < ROWS; r++) {
        printf("%2d ", r);
        for (int c = 0; c < COLS; c++) {
            printf("%c", canvas[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

// Listing helper
void listObjects() {
    if (objectCount == 0) {
        printf("\n(No objects currently added.)\n");
        return;
    }
    printf("\n--- Active Graphic Objects ---\n");
    for (int i = 0; i < objectCount; i++) {
        GraphicObject obj = objects[i];
        printf("[%d] ", obj.id);
        switch (obj.type) {
            case SHAPE_LINE:
                printf("Line from (%d, %d) to (%d, %d)\n", obj.data.line.x1, obj.data.line.y1, obj.data.line.x2, obj.data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                printf("Rectangle: Top-Left (%d, %d), Bottom-Right (%d, %d)\n", obj.data.rect.x1, obj.data.rect.y1, obj.data.rect.x2, obj.data.rect.y2);
                break;
            case SHAPE_CIRCLE:
                printf("Circle: Center (%d, %d), Radius %d\n", obj.data.circle.cx, obj.data.circle.cy, obj.data.circle.r);
                break;
            case SHAPE_TRIANGLE:
                printf("Triangle: P1(%d, %d), P2(%d, %d), P3(%d, %d)\n", obj.data.triangle.x1, obj.data.triangle.y1, obj.data.triangle.x2, obj.data.triangle.y2, obj.data.triangle.x3, obj.data.triangle.y3);
                break;
        }
    }
    printf("-------------------------------\n");
}

int findObjectIndex(int id) {
    for (int i = 0; i < objectCount; i++) {
        if (objects[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Add Object Function
void handleAddObject() {
    if (objectCount >= MAX_OBJECTS) {
        printf("Error: Maximum object limit reached (%d).\n", MAX_OBJECTS);
        return;
    }

    printf("\n--- Add New Object ---\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    int type = getIntInput("Choose shape type (1-4): ", 1, 4);

    GraphicObject newObj;
    newObj.id = nextId++;
    newObj.type = (ShapeType)type;

    switch (newObj.type) {
        case SHAPE_LINE:
            printf("Enter Line parameters:\n");
            newObj.data.line.x1 = getIntInput("  Start X (0-49): ", 0, COLS - 1);
            newObj.data.line.y1 = getIntInput("  Start Y (0-19): ", 0, ROWS - 1);
            newObj.data.line.x2 = getIntInput("  End X (0-49): ", 0, COLS - 1);
            newObj.data.line.y2 = getIntInput("  End Y (0-19): ", 0, ROWS - 1);
            break;
        case SHAPE_RECTANGLE:
            printf("Enter Rectangle parameters:\n");
            newObj.data.rect.x1 = getIntInput("  Top-Left X (0-49): ", 0, COLS - 1);
            newObj.data.rect.y1 = getIntInput("  Top-Left Y (0-19): ", 0, ROWS - 1);
            newObj.data.rect.x2 = getIntInput("  Bottom-Right X (0-49): ", 0, COLS - 1);
            newObj.data.rect.y2 = getIntInput("  Bottom-Right Y (0-19): ", 0, ROWS - 1);
            break;
        case SHAPE_CIRCLE:
            printf("Enter Circle parameters:\n");
            newObj.data.circle.cx = getIntInput("  Center X (0-49): ", 0, COLS - 1);
            newObj.data.circle.cy = getIntInput("  Center Y (0-19): ", 0, ROWS - 1);
            newObj.data.circle.r = getIntInput("  Radius (1-50): ", 1, 50);
            break;
        case SHAPE_TRIANGLE:
            printf("Enter Triangle parameters:\n");
            newObj.data.triangle.x1 = getIntInput("  Vertex 1 X (0-49): ", 0, COLS - 1);
            newObj.data.triangle.y1 = getIntInput("  Vertex 1 Y (0-19): ", 0, ROWS - 1);
            newObj.data.triangle.x2 = getIntInput("  Vertex 2 X (0-49): ", 0, COLS - 1);
            newObj.data.triangle.y2 = getIntInput("  Vertex 2 Y (0-19): ", 0, ROWS - 1);
            newObj.data.triangle.x3 = getIntInput("  Vertex 3 X (0-49): ", 0, COLS - 1);
            newObj.data.triangle.y3 = getIntInput("  Vertex 3 Y (0-19): ", 0, ROWS - 1);
            break;
    }

    objects[objectCount++] = newObj;
    printf("Successfully added object with ID %d!\n", newObj.id);
}

// Modify Object Function
void handleModifyObject() {
    if (objectCount == 0) {
        printf("\nNo objects to modify.\n");
        return;
    }

    listObjects();
    int id = getIntInput("Enter the ID of the object to modify: ", 1, nextId - 1);
    int idx = findObjectIndex(id);

    if (idx == -1) {
        printf("Error: Object with ID %d does not exist.\n", id);
        return;
    }

    GraphicObject *obj = &objects[idx];
    printf("\nModifying Object [%d]:\n", obj->id);

    switch (obj->type) {
        case SHAPE_LINE:
            printf("Enter new Line parameters:\n");
            obj->data.line.x1 = getIntInput("  Start X (0-49): ", 0, COLS - 1);
            obj->data.line.y1 = getIntInput("  Start Y (0-19): ", 0, ROWS - 1);
            obj->data.line.x2 = getIntInput("  End X (0-49): ", 0, COLS - 1);
            obj->data.line.y2 = getIntInput("  End Y (0-19): ", 0, ROWS - 1);
            break;
        case SHAPE_RECTANGLE:
            printf("Enter new Rectangle parameters:\n");
            obj->data.rect.x1 = getIntInput("  Top-Left X (0-49): ", 0, COLS - 1);
            obj->data.rect.y1 = getIntInput("  Top-Left Y (0-19): ", 0, ROWS - 1);
            obj->data.rect.x2 = getIntInput("  Bottom-Right X (0-49): ", 0, COLS - 1);
            obj->data.rect.y2 = getIntInput("  Bottom-Right Y (0-19): ", 0, ROWS - 1);
            break;
        case SHAPE_CIRCLE:
            printf("Enter new Circle parameters:\n");
            obj->data.circle.cx = getIntInput("  Center X (0-49): ", 0, COLS - 1);
            obj->data.circle.cy = getIntInput("  Center Y (0-19): ", 0, ROWS - 1);
            obj->data.circle.r = getIntInput("  Radius (1-50): ", 1, 50);
            break;
        case SHAPE_TRIANGLE:
            printf("Enter new Triangle parameters:\n");
            obj->data.triangle.x1 = getIntInput("  Vertex 1 X (0-49): ", 0, COLS - 1);
            obj->data.triangle.y1 = getIntInput("  Vertex 1 Y (0-19): ", 0, ROWS - 1);
            obj->data.triangle.x2 = getIntInput("  Vertex 2 X (0-49): ", 0, COLS - 1);
            obj->data.triangle.y2 = getIntInput("  Vertex 2 Y (0-19): ", 0, ROWS - 1);
            obj->data.triangle.x3 = getIntInput("  Vertex 3 X (0-49): ", 0, COLS - 1);
            obj->data.triangle.y3 = getIntInput("  Vertex 3 Y (0-19): ", 0, ROWS - 1);
            break;
    }

    printf("Successfully modified object %d!\n", id);
}

// Delete Object Function
void handleDeleteObject() {
    if (objectCount == 0) {
        printf("\nNo objects to delete.\n");
        return;
    }

    listObjects();
    int id = getIntInput("Enter the ID of the object to delete: ", 1, nextId - 1);
    int idx = findObjectIndex(id);

    if (idx == -1) {
        printf("Error: Object with ID %d does not exist.\n", id);
        return;
    }

    // Shift elements left to remove the object
    for (int i = idx; i < objectCount - 1; i++) {
        objects[i] = objects[i + 1];
    }
    objectCount--;
    printf("Successfully deleted object with ID %d!\n", id);
}

int main() {
    printf("==========================================\n");
    printf("          2D GRAPHICS EDITOR IN C         \n");
    printf("==========================================\n");

    while (1) {
        renderCanvas();
        displayCanvas();

        printf("--- Editor Menu ---\n");
        printf("1. Add Object\n");
        printf("2. Modify Object\n");
        printf("3. Delete Object\n");
        printf("4. List Active Objects\n");
        printf("5. Exit\n");

        int choice = getIntInput("Enter your choice (1-5): ", 1, 5);
        if (choice == 5) {
            printf("\nExiting Graphics Editor. Goodbye!\n");
            break;
        }

        switch (choice) {
            case 1:
                handleAddObject();
                break;
            case 2:
                handleModifyObject();
                break;
            case 3:
                handleDeleteObject();
                break;
            case 4:
                listObjects();
                // Pause so user can read list
                printf("\nPress Enter to continue...");
                while (getchar() != '\n');
                break;
        }
    }

    return 0;
}
