// Masjid Putera

#include <GL/glut.h>
#include <cmath>
#include <vector>
//#include <iostream> // Include for debugging output

const int numSegments = 36; // Number of rotations step
const int numCurvePoints = 10; // Number of points on profile curve
const float radius = 0.13f; //0.5f
const float height = 0.4f; //1.0f
// Middle Dome (Bigger)
const float middleDomeRadius = 0.9f;  // Larger radius for the middle dome
const float middleDomeHeight = 0.9f;   // Adjust height for the middle dome
std::vector<float> pointsCylinder; // Stores x, y as a flat array
std::vector<float> pointsDome; // Stores x, y as a flat array


// drawing quadrilateral where the domes will be resting on
void drawQuadrilateral(float xOffset, float yOffset, float widthBottom, float widthTop, float height) {
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.75f, 0.7f); // Brown color for the quadrilateral

    // Bottom-left vertex
    glVertex3f(xOffset - widthBottom / 2.0f, yOffset, 0.0f);
    // Bottom-right vertex
    glVertex3f(xOffset + widthBottom / 2.0f, yOffset, 0.0f);
    // Top-right vertex
    glVertex3f(xOffset + widthTop / 2.0f, yOffset + height, 0.0f);
    // Top-left vertex
    glVertex3f(xOffset - widthTop / 2.0f, yOffset + height, 0.0f);

    glEnd();
}

// Rectangle face of supporting base
void drawRectangle(float xOffset, float yOffset, float width, float height) {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.75f, 0.8f); // Pinkish color for the rectangle (or choose your preferred color)

    // Bottom-left vertex
    glVertex3f(xOffset - width / 2.0f, yOffset, 0.0f);
    // Bottom-right vertex
    glVertex3f(xOffset + width / 2.0f, yOffset, 0.0f);
    // Top-right vertex
    glVertex3f(xOffset + width / 2.0f, yOffset + height, 0.0f);
    // Top-left vertex
    glVertex3f(xOffset - width / 2.0f, yOffset + height, 0.0f);

    glEnd();
}

// diamond-shaped decorations on rectangle face
void drawDiamond(float xOffset, float yOffset, float size) {
    glBegin(GL_POLYGON);
    glColor3f(0.8f, 0.6f, 0.6f); // Grey color for the diamond

    // Top vertex
    glVertex3f(xOffset, yOffset + size, 0.0f);
    // Right vertex
    glVertex3f(xOffset + size, yOffset, 0.0f);
    // Bottom vertex
    glVertex3f(xOffset, yOffset - size, 0.0f);
    // Left vertex
    glVertex3f(xOffset - size, yOffset, 0.0f);

    glEnd();
}

// Function to generate a semicircular profile curve for sphere
void generateProfileCurveSphere(float sphereRadius) {
    pointsDome.clear(); // Reuse pointsDome for simplicity
    for (int i = 0; i < numCurvePoints; i++) {
        float t = (float)i / (numCurvePoints - 1); // Normalize 0 to 1
        float angle = t * M_PI; // From 0 to p (half-circle)
        float x = sphereRadius * cos(angle); // Horizontal radius
        float y = sphereRadius * sin(angle); // Vertical radius
        pointsDome.push_back(x);
        pointsDome.push_back(y);
    }
}

// Function to draw a tall and thin triangle
void drawTriangle(float xOffset, float yOffset, float height, float width) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.8f, 0.0f); // Yellow color for the triangle

    // Bottom-left vertex
    glVertex3f(xOffset - width / 2.0f, yOffset, 0.0f);
    // Bottom-right vertex
    glVertex3f(xOffset + width / 2.0f, yOffset, 0.0f);
    // Top vertex
    glVertex3f(xOffset, yOffset + height, 0.0f);

    glEnd();
}

// Function to draw spinning sphere using...
void drawSpinningSphere(float xOffset, float yOffset, float sphereRadius) {
    float angleStep = 2.0f * M_PI / numSegments;

    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= numSegments; i++) {
        float angle = i * angleStep;
        float cosA = cos(angle);
        float sinA = sin(angle);

        for (int j = 0; j < numCurvePoints; j++) {
            float x = sphereRadius * pointsDome[2 * j]; // Apply sphereRadius scaling
            float y = sphereRadius * pointsDome[2 * j + 1]; // Apply sphereRadius scaling

            // Compute rotated positions
            float xRot = x * cosA + xOffset;
            float zRot = x * sinA;
            float yPos = y + yOffset;

            glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for sphere
            glVertex3f(xRot, yPos, zRot);
        }
    }
    glEnd();
}

// Function to generate a profile curve (half-circle = cylinder)
void generateProfileCurveCylinder() {
    pointsCylinder.clear(); // Clear the vector before generating new points
    for (int i = 0; i < numCurvePoints; i++) {
        float t = (float)i / (numCurvePoints - 1); // Normalize 0 to 1, ensures points are evenly distributed 
        float x = radius; // Cylinder shape
        float y = (t - 0.5f) * height; // Stretching along Y-axis

        pointsCylinder.push_back(x); // Store X
        pointsCylinder.push_back(y); // Store Y
    }
}

// Function to generate a dome profile curve (half-sphere)
void generateProfileCurveDome(float domeRadius, float domeHeight) {
    pointsDome.clear();
    for (int i = 0; i < numCurvePoints; i++) {
        float t = (float)i / (numCurvePoints - 1); // 0 to 1
        float angle = t * (M_PI / 2.0f); // From 0 to p/2
        float x = domeRadius * cos(angle); // hemisphere side profile
        float y = domeHeight * sin(angle); // vertical curvature
        pointsDome.push_back(x);
        pointsDome.push_back(y);
    }
}

// Function to spin the profile curve around the Y-axis (cylinder)
void drawSpinning(float xOffset, const std::vector<float>& points) { // added xOffset parameter
    float angleStep = 2.0f * M_PI / numSegments;

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= numSegments; i++) {
        float angle = i * angleStep;
        float cosA = cos(angle);
        float sinA = sin(angle);

        for (int j = 0; j < numCurvePoints; j++) {
            float x = points[2 * j]; // Get x
            float y = points[2 * j + 1]; // Get y
            
            // Compute rotated positions
            float x1 = x * cosA + xOffset; // Add xOffset to x-coordinate
            float z1 = x * sinA;
            float x2 = x * cos(angle + angleStep) + xOffset; // Add xOffset to x-coordinate
            float z2 = x * sin(angle + angleStep);
            
            float redIntensity = 0.8f + (y / height) * 0.2f; // Adjust 0.8f and 0.2f for red intensity
            float pinkIntensity = 0.6f + (y / height) * 0.4f;
            // Blending red and pink
            glColor3f(redIntensity, pinkIntensity, pinkIntensity); // Adjust 0.4f for blue component

            glVertex3f(x1, y, z1);
            glVertex3f(x2, y, z2);
        }
    }
    glEnd();
}

// Function to draw a spinning dome
void drawDome(float xOffset, float domeRadius, float domeHeight) {
    float angleStep = 2.0f * M_PI / numSegments;

    glBegin(GL_TRIANGLE_FAN);
    float apexY = height / 2.0f + domeHeight;
    glColor3f(0.6f, 0.4f, 0.4f); // Center color
    glVertex3f(xOffset, apexY, 0.0f); // Apex of dome

    for (int i = 0; i <= numSegments; i++) {
        float angle = i * angleStep;
        float cosA = cos(angle);
        float sinA = sin(angle);

        for (int j = 0; j < numCurvePoints; j++) {
            float x = pointsDome[2 * j];
            float y = pointsDome[2 * j + 1];

            float xRot = x * cosA + xOffset;
            float zRot = x * sinA;
            float yPos = y + height / 2.0f;

            float redIntensity = 0.4f + (y / domeHeight) * 0.1f;
            float pinkIntensity = 0.3f + (y / domeHeight) * 0.1f;
            glColor3f(redIntensity, pinkIntensity, pinkIntensity);

            glVertex3f(xRot, yPos, zRot);
        }
    }
    glEnd();
}

//function to display 3D elements
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f); // Move the camera back
    glRotatef(5.0, 1.0f, 0.0f, 0.0f); // Rotate for better view
    
    // Draw the base quadrilateral
    drawQuadrilateral(0.0f, -0.3f, 2.0f, 1.3f, 0.4f);
    
    // Draw the rectangle directly below the quadrilateral
    drawRectangle(0.0f, -0.8f, 2.0f, 0.5f);
    
    // Temporarily disable depth testing to ensure diamonds are visible
    glDisable(GL_DEPTH_TEST);
    // Calculate positions for diamonds
    float rectangleWidth = 2.0f;  // Width of the pink rectangle
    float diamondSize = 0.1f;     // Size of each diamond
    float spacing = rectangleWidth / 6.0f; // Equal spacing (divide by 6 for gaps)
    float startX = -rectangleWidth / 2.0f + spacing; // Start position for first diamond

    for (int i = 0; i < 5; i++) {
        float xPos = startX + i * spacing; // Position of current diamond
        drawDiamond(xPos, -0.6f, diamondSize); // Draw diamond slightly above rectangle center
    }
    glEnable(GL_DEPTH_TEST); // Re-enable depth testing
	
	drawSpinning(-0.6f, pointsCylinder); // 1st cylinder for mini dome shifted left
    drawDome(-0.6f, radius, 0.3f); // Dome on left cylinder
    drawSpinningSphere(-0.6f, height / 2.0f + 0.3f + 0.01f, 0.35f); // Yellow sphere on left dome
    drawTriangle(-0.6f, height / 2.0f + 0.3f + 0.05f + 0.01f, 0.3f, 0.02f); // Tall, thin triangle on left dome
    
    // Draw the new, larger dome structure in the middle
	drawSpinning(0.0f, pointsCylinder); // Cylinder for middle dome
	drawDome(0.0f, middleDomeRadius, middleDomeHeight); // Dome on middle cylinder
	drawSpinningSphere(0.0f, height / 2.0f + middleDomeHeight + 0.01f, 0.35f); // Larger yellow sphere in the middle dome
	drawTriangle(0.0f, height / 2.0f + middleDomeHeight + 0.05f + 0.01f, 0.3f, 0.02f); // Tall, thin triangle on middle dome
	
	drawSpinning(0.6f, pointsCylinder); // 2nd cylinder for mini dome shifted right
    drawDome(0.6f, radius, 0.3f);  // Dome on right cylinder
    drawSpinningSphere(0.6f, height / 2.0f + 0.3f + 0.01f, 0.25f); // Yellow sphere on right dome
    drawTriangle(0.6f, height / 2.0f + 0.3f + 0.05f + 0.01f, 0.3f, 0.02f); // Tall, thin triangle on right dome
 
    glEnable(GL_DEPTH_TEST); // enables depth testing, which ensures that objects closer to the camera appear in front of objects that are farther away/ OpenGL initializationjected onto the 2D screen              
	
    glLoadIdentity(); // resets the current matrix to the identity matrix
    gluPerspective(45.0, 1.0, 0.1, 10.0); //sets up a perspective projection matrix
    glMatrixMode(GL_MODELVIEW); //Defines object transformations
    
    glutSwapBuffers();
}

// OpenGL initialization projected onto the 2D screen
void init() {
    glEnable(GL_DEPTH_TEST); // enables depth testing, which ensures that objects closer to the camera appear in front of objects that are farther away
    glMatrixMode(GL_PROJECTION); //Defines how the 3D world is projected onto the 2D screen
    glLoadIdentity(); // resets the current matrix to the identity matrix
    gluPerspective(45.0, 1.0, 0.1, 10.0); //sets up a perspective projection matrix
    glMatrixMode(GL_MODELVIEW); //Defines object transformations
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1400, 800); // display window size
    glutCreateWindow("Masjid Putra");

    init();
    generateProfileCurveCylinder();
    //generateProfileCurveSphere(0.1); // Smaller radius for yellow spheres
    generateProfileCurveDome(radius, 0.3f);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
