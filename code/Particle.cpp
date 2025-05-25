#include "Particle.h"

// +========================+
// |	PUBLIC FUNCTIONS	|
// +========================+
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
    : m_A(2, numPoints), m_ttl(TTL), m_numPoints(numPoints)
{
    float randFraction = (float)rand() / RAND_MAX;  // create fraction between 0-1
    m_radiansPerSec = randFraction * M_PI;
    m_cartesianPlane.setCenter(0, 0);
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);
    // cout << "Coordinate center: (" << m_centerCoordinate.x << ", " << m_centerCoordinate.y << ")" << endl;

    m_vx = rand() % 401 + 100; // Random velocity between 100 and 500
    m_vy = rand() % 401 + 100; // Random velocity between 100 and 500
    m_vx = (rand() % 2 == 0) ? m_vx : m_vx * -1; // Randomizes m_vx to be positive or negative

    // === Intialize color selection, feel free to change ===
    
    /*CODE ADDED 5/15/2025 5:42PM*/
    /*enum { RED, BLUE, TRANS, YELLOW, WHITE, CYAN, MAGENTA, GREEN }ParticleColors;
    int randInt = rand() % 4 + RED;
    switch (randInt) 
    {
        case RED: {m_color1 = Color::Red; break; }
        case BLUE: {m_color1 = Color::Blue; break; }
        case TRANS: {m_color1 = Color::Transparent; break; }
        case YELLOW: {m_color1 = Color::Yellow; break; }
        default: {m_color1 = Color::White; }
    }
    randInt = rand() % 4 + WHITE;
    switch (randInt)
    {
        case WHITE: {m_color2 = Color::White; break; }
        case CYAN: {m_color2 = Color::Cyan; break; }
        case MAGENTA: {m_color2 = Color::Magenta; break; }
        case GREEN: {m_color2 = Color::Green; break; }
        default: {m_color2 = Color::Black; }
    }
    */
    /*END CODE ADDED 5/15/2025 5:42PM*/

    //Original Color Scheme
    m_color1 = Color(252, 193, 26);
    m_color2 = Color::Yellow;

    m_lines.setPrimitiveType(TriangleFan);
    m_lines.resize(m_numPoints + 1);
   
    m_lines[0].color = m_color1;
    for (int j = 1; j <= m_numPoints; j++)
    {
        m_lines[j].color = m_color2;
        m_lines[j].position = Vector2f(0, 0);
    }

    double theta = randFraction / M_PI;
    double dTheta = 2 * M_PI / (numPoints - 1);

    // Logic for making star-like shapes
    double outerRadius = rand() % 63 + 10; // A random distance - determines length of each 'tip' of the star
    double innerRadius = outerRadius / 2; // A fraction of outer radius - determines depth of each 'dip' of the star

    for (int j = 0; j < numPoints; j++)
    {
        double r = (j % 2 == 0) ? outerRadius : innerRadius; // Alternate between drawing 'tip' then 'dip' for each vertex
        double dx = r * cos(theta);
        double dy = r * sin(theta);

        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;
        theta += dTheta;
    }
}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
    Vector2f center(target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane));
    m_lines[0].position = center;

    for (int j = 1; j <= m_numPoints; j++)
    {
        float x = (float)m_A(0, j - 1);
        float y = (float)m_A(1, j - 1);

        Vector2f matrixCoord(x, y);
        Vector2f mappedCoord(target.mapCoordsToPixel(matrixCoord, m_cartesianPlane));
        m_lines[j].position = mappedCoord;
    }

    target.draw(m_lines);
    //cout << "Exit draw\n";
}

void Particle::update(float dt)
{
    
    // Fade based on how much lifetime is left
    float ttlRatio = max(0.0f, m_ttl / m_initialTTL);  // 1.0 -> 0.0
    Uint8 newAlpha = static_cast<Uint8>(255 * ttlRatio);

    // Lowers the alpha value (opacity) smoothly
    m_color1.a = max((int)newAlpha, 0);
    m_color2.a = max((int)newAlpha, 0);

    // Updates the particle drawing with current opacity
    m_lines[0].color = m_color1;
    for (int j = 1; j <= m_numPoints; j++)
    {
        m_lines[j].color = m_color2;
    }
    
    m_ttl -= dt;

    rotate(dt * m_radiansPerSec);
    scale(SCALE);

    float dx = m_vx * dt;
    m_vy -= G * dt;			// Apply gravity (G) to vertical movement
    float dy = m_vy * dt;

    translate(dx, dy);
}

// +====================+
// |	UNIT TESTS		|
// +====================+

bool Particle::almostEqual(double a, double b, double eps)
{
    return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }


    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}

// +========================+
// |	PRIVATE FUNCTIONS	|
// +========================+

///rotate Particle by theta radians counter-clockwise
///construct a RotationMatrix R, left mulitply it to m_A
void Particle::rotate(double theta)
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    RotationMatrix R(theta);
    m_A = R * m_A;

    translate(temp.x, temp.y);
}

///Scale the size of the Particle by factor c
///construct a ScalingMatrix S, left multiply it to m_A
void Particle::scale(double c)
{
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

    ScalingMatrix S(c);
    m_A = S * m_A;

    translate(temp.x, temp.y);
}

///shift the Particle by (xShift, yShift) coordinates
///construct a TranslationMatrix T, add it to m_A
void Particle::translate(double xShift, double yShift)
{
    int nCols = m_A.getCols();
    TranslationMatrix T(xShift, yShift, nCols);
    m_A = T + m_A;
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}
