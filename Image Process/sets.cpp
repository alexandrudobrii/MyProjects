#include "sets.h"
#include "mpi.h"

int main (int argc, char *argv[]) { 
  int numtasks, rank, tag = 1;
  int **pgm = NULL, split;
  double start_x, end_x, start_y, end_y;
  MPI_Status Stat;
  
  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* daca este MASETR se face citirea din fisier
   * si isi incepe calulul pentru propria sa submatrice*/
  if (rank == MASTER) {
    read_from_file(argv[1]);

    pgm = MPI_Alloc_Matrix(width,height);

    split = width/numtasks;
    start_x = x_min;
    end_x = start_x + split*resolution;
    start_y = y_min;
    end_y = y_max;

    gen_set(start_x, end_x, start_y, end_y);
  }

  /* se trimit informatiile citite din fisier la celelalte procese */
  MPI_Bcast (&set, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&x_min, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&x_max, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&y_min, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&y_max, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&a, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&b, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&resolution, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&max_steps, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
  MPI_Bcast (&split, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

  /* daca nu este MASTER, procesul isi calculeaza propriul interval */
  if (rank != MASTER) {
    start_x = x_min + rank*split*resolution;
    end_x = start_x + split*resolution;
    start_y = y_min;
    end_y = y_max;

    gen_set(start_x, end_x, start_y, end_y); 
  }

  /* daca este MASTER, se colecteaza informatiile de la celalte procese */
  if (rank == MASTER) {
    int n, m, start;

    for (int i = 0; i < numtasks; i++) {
      MPI_Recv(&n, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
      MPI_Recv(&m, 1, MPI_INT, i, tag, MPI_COMM_WORLD, &Stat);
      int **buffer = MPI_Recv_Matrix(n,m,i);     
      start = split * i;
     
      /* se memoreaza informatia primita */
      for (int j = 0; j < n; j++) {
        for (int k = 0; k < m; k++) {
          pgm[start][k] = buffer[j][k];
        }
        start++;
      }
    }

    /* se scrie in fisierul de iesire */
    print_set(pgm, argv[2]);
    MPI_Free_Matrix(pgm, width);
  }

  MPI_Finalize();

  return 0;
}

/* functie care citeste datele din fisier */
void read_from_file (const char *filename) {
  std::ifstream in(filename);
  
  in >> set;
  in >> x_min >> x_max >> y_min >> y_max;
  in >> resolution;
  in >> max_steps;

  if (set == JULIA) {
    in >> a >> b;
  }

  /* se determinca dimensiunile imaginii */
  width = round((x_max-x_min)/resolution);
  height = round((y_max-y_min)/resolution);

  in.close();
}

/* functie ce genereaza multimea julia */
void gen_julia_set (double start_x, double end_x, 
                    double start_y, double end_y) {
  int i = -1, j = 0, iteration;
  double x, y;
  
  int width = round((end_x-start_x)/resolution);
  int height = round((end_y-start_y)/resolution);
  int **pgm = MPI_Alloc_Matrix(width+1, height+1);

  for (double x0 = start_x; x0 < end_x; x0 += resolution) {
    i++;
    for (double y0 = start_y; y0 < end_y; y0 += resolution) {
      iteration = 0;
      x = x0; 
      y = y0;
      while (x*x + y*y < 4.0f && iteration < max_steps) {
        double xtemp = x*x - y*y + a;
        y = 2*x*y + b;
        x = xtemp;  
        iteration++;
      }
      pgm[i][j++] = iteration;
    }
    j = 0;
  }

  /* ficare proces trimite la MASTER matricea sa */
  MPI_Send(&width, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD); 
  MPI_Send(&height, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD); 
  MPI_Send_Matrix(pgm, width, height); 
}

/* functie ce genereaza multimea mandelbrot */
void gen_mandelbrot_set(double start_x, double end_x, 
                        double start_y, double end_y) {
  int i = -1, j = 0, iteration;
  double x, y;  
  
  int width = round((end_x-start_x)/resolution);
  int height = round((end_y-start_y)/resolution);
  int **pgm = MPI_Alloc_Matrix(width+1, height+1);

  for (double x0 = start_x; x0 < end_x; x0 += resolution) {
    i++;
    for (double y0 = start_y; y0 < end_y; y0 += resolution) {
      x = y = 0.0f;
      iteration = 0;
      while (x*x + y*y < 4.0f && iteration < max_steps) {
        double xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;  
        iteration++;
      }
      pgm[i][j++] = iteration;
    }
    j = 0;
  }

  /* ficare proces trimite la MASTER matricea sa */
  MPI_Send(&width, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD); 
  MPI_Send(&height, 1, MPI_INT, MASTER, 1, MPI_COMM_WORLD); 
  MPI_Send_Matrix(pgm, width, height);
}

/* functie ce alege tipul multimii */
void gen_set (double start_x, double end_x, double start_y, double end_y) {
  
  if (set == JULIA) {
    gen_julia_set(start_x, end_x, start_y, end_y);
  }
  
  if (set == MANDELBROT) {
    gen_mandelbrot_set(start_x, end_x, start_y, end_y);
  }
}

/* functie ce creeaza imaginea .pgm in functie de multime */
void print_set (int **pgm, const char *filename) {
  std::ofstream out(filename);
  
  out << "P2\n\r";
  out << width << " " << height << "\n\r";
  out << NUM_COLORS-1 << "\n\r";

  for (int i = height-1; i >= 0; i--) {
    for (int j = 0; j < width; j++) {
      out << pgm[j][i] << " ";
    }
    out << "\n\r";
  }

  out.close();
}

/* functie de alocare a memoriei pentru o matrice */
int **MPI_Alloc_Matrix(int n, int m) {
  int **array;

  array = new int*[n];
  for (int i = 0; i < n; i++) {
    array[i] = new int[m];
  }

  return array;
}

/* functie de eliberare a memoriei ocupate de o matrice */
void MPI_Free_Matrix (int **buffer, int n) {
    for (int i = 0; i < n; i++) {
      delete[] buffer[i];
    }
    delete[] buffer;
}

/* functie care primeste o matrice linie cu linie */
void MPI_Send_Matrix (int **buffer, int n, int m) {
  for (int i = 0; i < n; i++) {
    MPI_Send(buffer[i], m, MPI_INT, MASTER, 1, MPI_COMM_WORLD);
  }
} 

/* functie care trimite o matrice linie cu linie */
int **MPI_Recv_Matrix (int n, int m, int source) {
  MPI_Status Stat;
  int **buffer = MPI_Alloc_Matrix(n,m);
  
  for (int i = 0; i < n; i++) {
    MPI_Recv(buffer[i], m, MPI_INT, source, 1, MPI_COMM_WORLD, &Stat);
  }
   
  return buffer;
} 