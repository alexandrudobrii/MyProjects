#include "Spacecraft.h"

/* constructorul clasei Spacecraft */
Spacecraft::Spacecraft() : mesh(NULL) { }

/* constructorul clasei Spacecraft */
Spacecraft::Spacecraft (const char *filename) : blending(true) {
  mesh = ReadOffFile(filename);
}

/* destructorul clasei Spacecraft */
Spacecraft::~Spacecraft() {
  delete mesh;
}

/* metoda care seteaza transparenta scutului navei */
void Spacecraft::set_transparency (float transparency) {
    this->transparency = transparency;
}

/* metoda care scade transparenta scutului navei */
void Spacecraft::decrease_transparency (float transparency) {
    if (this->transparency > 0) {
      this->transparency -= transparency;
    }
}

/* metoda care verifica daca nava este distrusa */
bool Spacecraft::is_destroyed() {
  return (float)this->transparency < 0;
}

/* metoda ce activeaza efectul de transparenta */
void Spacecraft::enable_blending() {
  blending = true;
}

/* metoda ce dezactiveaza efectul de transparenta */
void Spacecraft::disable_blending() {
  blending = false;
}

/* metoda ce deseneaza nava in pozitia specificata */
void Spacecraft::draw() {
 
  glTranslatef(position.x, position.y, position.z);
  glScalef(SCALE, SCALE, SCALE);
  glRotatef(-90,1,0,0);
  glRotatef(-90,0,0,1);
  
  // Set lights
  static GLfloat light0_position[] = { 3.0, 4.0, 5.0, 0.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  static GLfloat light1_position[] = { -3.0, -2.0, -3.0, 0.0 };
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

  // Set material
  static GLfloat material[] = { 1.0, 0.5, 0.5, 1.0 };
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material); 

  // Draw faces
  for (int i = 0; i < mesh->nfaces; i++) {
    Face& face = mesh->faces[i];
    glBegin(GL_POLYGON);
    glColor3f(BLUE);
    glNormal3fv(face.normal);
    for (int j = 0; j < face.nverts; j++) {
      Vertex *vert = face.verts[j];
      glVertex3f(vert->x, vert->y, vert->z);
    }
    glEnd();
  }

  /* se deseneaza scutul */
  if (blending) {
    glEnable(GL_BLEND);
    glAlphaFunc(GL_EQUAL, 1);
    glDepthMask(GL_FALSE);

    glColor4f(YELLOW, transparency);
    glTranslatef(0,0.5,0.1);
    glutSolidSphere(0.7,100,100); 

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glAlphaFunc(GL_LESS, 1);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
  }
}

/* metoda ce simuleaza mutarea navei in functie de tasa apasata */
Vector3D Spacecraft::simulate (float dist, int id) {
  if (id == 0) {
    return position + forward*dist;
  }

  if (id == 1) {
    return position + right*dist;
  }

  if (id == 2) {
    return position + up*dist;
  }
} 

/* metoda ce muta nava in functie de tasa apasata */
void Spacecraft::move (unsigned char& key) {
  switch (key) {
    case 'q':
      this->translate_Forward(SPEED);
      break;
    case 'w':
      this->translate_Up(SPEED);
      break;
    case 'e':
      this->translate_Forward(-SPEED);
      break;
    case 's':
      this->translate_Up(-SPEED);
      break;
    case 'a':
      this->translate_Right(SPEED);
      break;
    case 'd':
      this->translate_Right(-SPEED);
      break;
  }
}

/* metoda care verifica daca nava iese afara din decor */
bool Spacecraft::is_colision (char key) {
  Vector3D position;

  /* se simuleaza mutarea jucatoului intr-o anumita directie */
  switch (key) {
    case 'q':
      position = this->simulate(SPEED,FORWARD);
      break;
    case 'e':
      position = this->simulate(-SPEED,BACK);
      break;
    case 'd':
      position = this->simulate(-SPEED,RIGHT);
      break;
    case 'a':
      position = this->simulate(SPEED,LEFT);
      break;
    case 'w':
      position = this->simulate(SPEED,UP);
      break;
    case 's':
      position = this->simulate(-SPEED,DOWN);
      break;
    default:
      return false;
      break;
  }

  return out_of_boarders(position);
}

/* metoda care verifica daca exista vreo coliziune intre nava si vreun atsteroid */
bool Spacecraft::is_impact (Asteroid asteroid) {
  Vector3D relPos = this->get_position() - asteroid.get_position();
  float dist = relPos.x * relPos.x + relPos.y * relPos.y + relPos.z * relPos.z;
  
  return dist <= RADIUS * RADIUS;
}

bool Spacecraft::out_of_boarders (Vector3D position) {
  return fabs(position.x) > 18 || fabs(position.y) > 9
              || position.z > 45.0 || position.z < 35.0;
}

/* functie ce realizeaza citirea din fisier a navei */
Mesh *ReadOffFile(const char *filename) {
  int i;

  // Open file
  FILE *fp;
  if (!(fp = fopen(filename, "r"))) {
    fprintf(stderr, "Unable to open file %s\n", filename);
    return 0;
  }

  // Allocate mesh structure
  Mesh *mesh = new Mesh();
  if (!mesh) {
    fprintf(stderr, "Unable to allocate memory for file %s\n", filename);
    fclose(fp);
    return 0;
  }

  // Read file
  int nverts = 0;
  int nfaces = 0;
  int nedges = 0;
  int line_count = 0;
  char buffer[1024];
  while (fgets(buffer, 1023, fp)) {
    // Increment line counter
    line_count++;

    // Skip white space
    char *bufferp = buffer;
    while (isspace(*bufferp)) bufferp++;

    // Skip blank lines and comments
    if (*bufferp == '#') continue;
    if (*bufferp == '\0') continue;

    // Check section
    if (nverts == 0) {
      // Read header 
      if (!strstr(bufferp, "OFF")) {
        // Read mesh counts
        if ((sscanf(bufferp, "%d%d%d", 
                          &nverts, &nfaces, &nedges) != 3) || (nverts == 0)) {
          fprintf(stderr, "Syntax error reading header on line %d in file %s\n", line_count, filename);
          fclose(fp);
          return NULL;
        }

        // Allocate memory for mesh
        mesh->verts = new Vertex [nverts];
        assert(mesh->verts);
        mesh->faces = new Face [nfaces];
        assert(mesh->faces);
      }
    }
    else if (mesh->nverts < nverts) {
      // Read vertex coordinates
      Vertex& vert = mesh->verts[mesh->nverts++];
      if (sscanf(bufferp, "%f%f%f", &(vert.x), &(vert.y), &(vert.z)) != 3) {
        fprintf(stderr, "Syntax error with vertex coordinates on line %d in file %s\n", line_count, filename);
        fclose(fp);
        return NULL;
      }
    }
    else if (mesh->nfaces < nfaces) {
      // Get next face
      Face& face = mesh->faces[mesh->nfaces++];

      // Read number of vertices in face 
      bufferp = strtok(bufferp, " \t");
      if (bufferp) face.nverts = atoi(bufferp);
      else {
        fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
        fclose(fp);
        return NULL;
      }

      // Allocate memory for face vertices
      face.verts = new Vertex *[face.nverts];
      assert(face.verts);

      // Read vertex indices for face
      for (i = 0; i < face.nverts; i++) {
        bufferp = strtok(NULL, " \t");
        if (bufferp) face.verts[i] = &(mesh->verts[atoi(bufferp)]);
        else {
          fprintf(stderr, "Syntax error with face on line %d in file %s\n", line_count, filename);
          fclose(fp);
          return NULL;
        }
      }

      // Compute normal for face
      face.normal[0] = face.normal[1] = face.normal[2] = 0;
      Vertex *v1 = face.verts[face.nverts-1];
      for (i = 0; i < face.nverts; i++) {
        Vertex *v2 = face.verts[i];
        face.normal[0] += (v1->y - v2->y) * (v1->z + v2->z);
        face.normal[1] += (v1->z - v2->z) * (v1->x + v2->x);
        face.normal[2] += (v1->x - v2->x) * (v1->y + v2->y);
        v1 = v2;
      }

      // Normalize normal for face
      float squared_normal_length = 0.0;
      squared_normal_length += face.normal[0]*face.normal[0];
      squared_normal_length += face.normal[1]*face.normal[1];
      squared_normal_length += face.normal[2]*face.normal[2];
      float normal_length = sqrt(squared_normal_length);
      if (normal_length > 1.0E-6) {
        face.normal[0] /= normal_length;
        face.normal[1] /= normal_length;
        face.normal[2] /= normal_length;
      }
    }
    else {
      // Should never get here
      fprintf(stderr, "Found extra text starting at line %d in file %s\n", line_count, filename);
      break;
    }
  }

  // Check whether read all faces
  if (nfaces != mesh->nfaces) {
    fprintf(stderr, "Expected %d faces, but read only %d faces in file %s\n", nfaces, mesh->nfaces, filename);
  }

  // Close file
  fclose(fp);

  // Return mesh 
  return mesh;
}
