import java.io.*;

public class Main {
	static String[] words, docs;
	static int D, N, X;
	static int num_threads;
	static WorkPool map;
	static Writer out;
	
	/* metoda care citeste din fisierul de intrare datele necesare */
	static void read_from_file (String name, String output) throws IOException {
		Reader reader = new InputStreamReader(
                new FileInputStream(name),"US-ASCII");
		BufferedReader fin = new BufferedReader(reader);
		
		fin.readLine();
		words = fin.readLine().split(" ");
		D = Integer.parseInt(fin.readLine());
		N = Integer.parseInt(fin.readLine());
		X = Integer.parseInt(fin.readLine());
		int ND = Integer.parseInt(fin.readLine());
		
		docs = new String[ND];
		for (int i = 0; i < ND; i++) {
			docs[i] = fin.readLine();
		}
		
		/* se afiseaza in fisierul de iesire cuvintele cautate */
		out = new PrintWriter(new FileWriter(output));
		out.write("Rezultate pentru: (");
		for (int i = 0; i <  words.length; i++) {
			out.write(words[i]);
			
			if (i == words.length-1) {
				out.write(")\n\n");
			} else {
				out.write(", ");
			}
		}
	}
	
	static void close_file() throws IOException {
		out.close();
	}
	
	/* metoda care creeaza cate un task de tipul Map pentru fiecare fisier */
	static void create_partial_solutions (String name) throws IOException {
		File file = new File(name);
		
		for (int i = 0; i <= file.length()/D; i++) {
			map.putWork(new Mapp(name, i*D, D));
		}
	}
	
	static void run_threads (WorkPool workpool, int action) 
												  throws InterruptedException {
		Concurent.Worker[] worker;
		
		worker = new Concurent.Worker[num_threads];
		for (int i = 0; i < num_threads; i++) {
			worker[i] = new Concurent.Worker(workpool, action);
			worker[i].start();
		}
		
		for (int i = 0; i < num_threads; i++) {
			worker[i].join();
		}
	}
	
	/* metoda care creeaza mai multe thread-uri carora le atribuie task-uri 
	 * si in final, dupa combinarea tuturor rezultatelor de la fiecare task
	 * in parte se obtin frecventele cuvintelor cautate in fisierel de intrare*/
	static void MapReduce (int num_threads) 
									 throws IOException, InterruptedException {
		
		map = new WorkPool(num_threads);
		for (int i = 0; i < docs.length; i++) {
			create_partial_solutions(docs[i]);
		}
		
		run_threads(map, 0);
		run_threads(Concurent.reduce, 1);
		run_threads(Concurent.reduce, 2);
		
		Concurent.create_tasks();
		run_threads(Concurent.reduce, 3);
		close_file();
	}
	
	public static void main (String[] args) throws 
											IOException, InterruptedException {
		read_from_file(args[1], args[2]);
		
		num_threads = Integer.parseInt(args[0]);
		
		MapReduce(num_threads);
	}
}
