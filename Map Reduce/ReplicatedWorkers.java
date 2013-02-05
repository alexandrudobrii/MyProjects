import java.io.*;
import java.util.*;

/**
 * Clasa ce reprezinta o solutie partiala pentru problema de rezolvat. Aceste
 * solutii partiale constituie task-uri care sunt introduse in workpool.
 */

/* clasa ce reprezinta un cuvant, respectiv numarul de aparitii */
class PartialVector implements Comparable<PartialVector> {
	String word;
	int frequency;
	
	public PartialVector (String word, int frequency) {
		this.word = word;
		this.frequency = frequency;
	}
	
	public int compareTo(PartialVector obj) {
		return obj.frequency - this.frequency;
	}
}

/* clasa cu ajutorul careia se realizeaza operatia Map, care
 * primeste ca obiecte fisierul din care se citetse, pozitia
 * de unde se incepe citirea si numarul de caracatere de citit */
class Mapp {
	private String filename;
	private int offset;
	private int len;
	 
	public Mapp (String filename, int offset, int len) {
		this.filename = filename;
		this.offset = offset;
		this.len = len;
	}
	
	public String get_filename() {
		return filename;
	}
	
	/* metoda care citeste din fisierul de intrare D octeti */
	public String read_buffer() throws IOException {
		Reader reader = new InputStreamReader(
                new FileInputStream(filename),"US-ASCII");
		BufferedReader file = new BufferedReader(reader);
		char[] buffer = new char[20*len];
		int n_bytes, index1, index2;

		if (offset != 0) {
			offset--;
			len++;
		}
		
		/* se realizeaza citirea din fisier 
		 * incepand cu un anumit offset */
		file.skip(offset);
		n_bytes = file.read(buffer, 0, len);
		
		/* daca sirul citit se termina in mijlocul unui 
		 * cuvant, se memoreaza si cuvantul respectiv */
		index1 = n_bytes;
		if (buffer[n_bytes-1] != ' ') {
			while ((n_bytes = file.read(buffer, index1, 1)) != -1) {
 				if (buffer[index1] == ' ') {
 					break;
 				}
 				index1 += 1;
 			}
 		}
		
		/* daca sirul citit incepe in mijlocul unui 
		 * cuvant, se ignora cuvantul respectiv */
		index2 = 0;
		if (offset != 0) {		
			while (index2 < index1 && buffer[index2] != ' ') {
				index2 += 1;
			}
 		}
		
		file.close();
		return String.copyValueOf(buffer,index2,index1);
	}
	
	/* se extrag cuvintele ce contin numai litere */
	public String[] split (String buffer) {
		String[] splitBuffer = buffer.split("[^a-zA-Z]");
		
		/* se face conversia din litere mari in litere mici */
		for (int i = 0; i < splitBuffer.length; i++) {
			splitBuffer[i] = splitBuffer[i].toLowerCase();
		}
		
		return splitBuffer;
	}
}

/* clasa ce realizeaza operatie Reduce */
class Reduce {
	String filename;
	HashMap<String, PartialVector> list;
	
	public Reduce (String filename) {
		this.filename = filename;
	}
	
	public Reduce (String filename, HashMap<String, PartialVector> list) {
		this.list = list;
		this.filename = filename;
	}
}

/* clasa ce reprezinta memoria partajata pentru workeri */
class Concurent extends Main {
	public static Map<String, HashMap<String, PartialVector>> list 
					  = new HashMap <String, HashMap<String, PartialVector>>();
	public static WorkPool reduce = new WorkPool(num_threads);
	public static HashMap<String, Integer> 
									  num_words = new HashMap<String,Integer>();
	public static int count = 0;
	
	public static double aproximate (int freq, int num_words) {
		double result;
		
		result = (double)freq/num_words*100;
		
		return (double)Math.floor(result*100)/100;
	}
	
	/* metoda ce realizeaza afisarea in datelor in fisier */
	static void print (LinkedList<Integer>freq, String file)throws IOException {
		double result;
		
		/* daca s-au afisat frecventele pentru 
		 * X documnete se goleste workpool-ul */
		if (count+1 == X) {
			reduce.clear();
		} else {
			count++;
		}
		
		out.write(file + " (");
		for (int i = 0; i < freq.size(); i++) {
			result = aproximate(freq.get(i), num_words.get(file));
			out.write(String.format("%.2f", result));
			
			if (i == freq.size()-1) {
				out.write(")\n");
			} else {
				out.write(", ");
			}
		}
	}
	
	public static void create_tasks() {
		for (int i = 0; i < docs.length; i++) {
			reduce.putWork(new Reduce(docs[i]));
		}
	}
	
/**
 * Clasa ce reprezinta un thread worker.
 */	static class Worker extends Thread {
		WorkPool map;
		int action;
		
		public Worker(WorkPool map, int action) {
			this.map = map;
			this.action = action;
		}
	
	/**
	 * Procesarea unei solutii partiale. Aceasta poate implica generarea unor
	 * noi solutii partiale care se adauga in workpool folosind putWork().
	 * Daca s-a ajuns la o solutie finala, aceasta va fi afisata.
	 * @throws IOException 
	 */
		/* metoda care realizeaza extragerea cuvintelor din fiecare
		 * document impreuna cu numarul de aparitii al acestora */
		void processMap (Mapp ps) throws IOException {
			int count;
			String buffer = ps.read_buffer();
			String[] words = ps.split(buffer);
			HashMap<String, PartialVector> list;
			list = new HashMap<String, PartialVector>();

			for (int i = 0; i < words.length; i++) {
				String word = words[i];
				PartialVector pv = list.get(word);
			
				if (word.equals("") || pv != null) {
					continue;
				}
			
				count = 1;
				for (int j = 0; j < words.length; j++) {
					if (i != j && word.equals(words[j])) {
						count++;
					}
				}
			
				list.put(word, new PartialVector(word, count));
			}
			
			/* se adauga un nou task pentru operatia Reduce */
			reduce.putWork(new Reduce(ps.get_filename(), list));
		}
	
		/* metoda care combina solutiile generate de Map */
		void processReduce (Reduce ps) {
			List<PartialVector> pv 
							   = new ArrayList<PartialVector>(ps.list.values());
		
			for (int i = 0; i < pv.size(); i++) {
				String word = pv.get(i).word;
				HashMap<String, PartialVector> hm;
				PartialVector sol;
				
				synchronized (num_words) {
					int sum;
					if (num_words.get(ps.filename) != null) {
						sum = num_words.get(ps.filename);
						sum += pv.get(i).frequency;
					} else {
						sum = pv.get(i).frequency;
					}
					
					num_words.put(ps.filename, sum);
				}
				
				synchronized (list) {
					hm = list.get(ps.filename);
					
					if (hm != null) {
						sol = hm.get(word);
						if (sol != null) {
							sol.frequency += pv.get(i).frequency;
						} else {
							sol = new PartialVector(word, pv.get(i).frequency);
						}
					} else {
						hm = new HashMap<String, PartialVector>();
						sol = new PartialVector(word, pv.get(i).frequency);
					}
					
					hm.put(word, sol);
					list.put(ps.filename, hm);
				}
			}
		}
		
		/* metoda care extrage primele N cuvinte dupa frecventa */
		void combine_solutions (Reduce solution) {
			HashMap<String, PartialVector> hm;
			int count;
			
			hm = list.get(solution.filename);
			
			List<PartialVector> pv = new ArrayList<PartialVector>(hm.values());
			List<PartialVector> first = new ArrayList<PartialVector>();
			
			/* se sorteaza vectorul de cuvinte dupa frecventa */
			Collections.sort(pv);
			
			/* daca mai sunt cuvinte cu frecventa egala cu 
			 * cel de-al N-lea se memoreaza si acestea */
			count = N;
			while (count < pv.size() && 
						pv.get(N-1).frequency == pv.get(count).frequency) {
				count++;
			}
			first = pv.subList(0, count);
			
			/* se realizeaza impartirea cuvintelor pe fiecare fisier */
			hm = new HashMap<String, PartialVector>();
			for (int i = 0; i < first.size(); i++) {
				String word = first.get(i).word;
				int frequency = first.get(i).frequency;
				
				hm.put(word, new PartialVector(word, frequency));
			}
			
			list.put(solution.filename, hm);
		}
		
		/* metoda ce realizeaza cautarea si afisrea cuvintelor cautate */
		void search (Reduce solution) throws IOException {
			HashMap<String, PartialVector> hm;
			PartialVector key;
			LinkedList<Integer> freq = new LinkedList<Integer>();
			
			synchronized (words) {	
				for (int i = 0; i < words.length; i++) {
					hm = list.get(solution.filename);
					key = hm.get(words[i]);
					
					if (key == null) {
						break;
					}
					
					/* daca a fost gasit se memoreaza pentru afisare */
					freq.add(i, key.frequency);
				}

				/* se afiseaza frecventele pentru primele X documente */		
				if (freq.size() == words.length) {
					print(freq, solution.filename);
				}
			}
		}	
		
		/* metoda care porneste workerii pentru 
		 * operatia de Map, respectiv Reduce */
		public void run() {
			while (true) {
				if (this.action == 0) {
					Mapp ps = (Mapp)map.getWork();
					if (ps == null)
						break;
			
					try {
						processMap(ps);
					} catch (IOException e) {
						e.printStackTrace();
					}
				} else if (this.action == 1) {
					Reduce ps = (Reduce)reduce.getWork();
					if (ps == null) {
						break;
					}
							
					processReduce(ps);
				} else if (this.action == 2) {
					Reduce ps = (Reduce)reduce.getWork();
					if (ps == null) {
						break;
					}
							
					combine_solutions(ps);
				}  else if (this.action == 3) {
					Reduce ps = (Reduce)reduce.getWork();
					if (ps == null) {
						break;
					}
					try {
						search(ps);
					} catch (IOException e) {
						e.printStackTrace();
					}	
				}
			}
		}
	}
}


