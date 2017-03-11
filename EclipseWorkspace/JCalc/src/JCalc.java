import java.io.*;
import java.util.*;

public class JCalc {

	// Linked list of tokens of arithmetic sentence.
	// The list is populated by stream tokenizer.
	protected LinkedList<String> Tokens;

	public static void main(String[] args) {
		JCalc Jc = new JCalc();
		if (Jc.getInput())
			Jc.parseAndPrint();
	}

	// Constructor
	public JCalc() {
		Tokens = new LinkedList<String>();
	}

	public boolean getInput() {
		try {
			Reader Read = new BufferedReader(new InputStreamReader(System.in));
			StreamTokenizer StTok = new StreamTokenizer(Read);
			StTok.eolIsSignificant(true);
			StTok.ordinaryChar('/');
			int LastToken = StreamTokenizer.TT_EOL;
			boolean eof = false;
			do {
				int token = StTok.nextToken();
				switch (token) {
				case StreamTokenizer.TT_EOF:
					eof = true;
					break;
				case StreamTokenizer.TT_EOL:
					eof = true;
					break;
				case StreamTokenizer.TT_WORD:
					// No alpha symbols allowed
					System.out.println("Word: Error");
					return false;
				case StreamTokenizer.TT_NUMBER:
					if(LastToken == StreamTokenizer.TT_NUMBER)
						Tokens.add("+");
					Tokens.add(new Double(StTok.nval).toString());
					System.out.print(new Double(StTok.nval).toString());
					break;
				default:
					System.out.print((char) token);
					switch ((char) token) {
					case '+':
						Tokens.add("+");
						break;
					case '-':
						Tokens.add("-");
						break;
					case '*':
						Tokens.add("*");
						break;
					case '/':
						Tokens.add("/");
						break;
					default:
						// No other symbols allowed, arithmetic operations only.
						System.out.println("Symbol: Error");
						return false;
					}
				}
				LastToken = token;
			} while (!eof);
			// Simple sanity check
			String Last = Tokens.get(Tokens.size() - 1).toString();
			if (Last == "*" || Last == "/" || Last == "+" || Last == "-") {
				System.out.println(" The sentence cannot be ended with operation symbol.");
				return false;
			}
			System.out.print("=");
		} catch (IOException e) {
			System.out.println(e.toString());
		}
		return true;
	}

	public void parseAndPrint() {
		// High level precedence operations (* /)
		int index = 0;
		while (true) {
			int index_mult = Tokens.indexOf("*"); // Find first multiplication operation.
			int index_dev = Tokens.indexOf("/"); // Find first devision operation.
			if (index_mult < 0 && index_dev < 0) // No multiplication or devision operations remains in list. 
				break; // Exit from high level precedence loop an go to do low level. 
			if (index_mult < 0) // No multiplication operations remains in list. Do division operation.
				index = index_dev;
			else if (index_dev < 0) // No devision operations remains in list. Do multiplication operation.
				index = index_mult;
			else
				index = Integer.min(index_mult, index_dev); // Found both operations. Do first found.
			Double d;
			if (index == index_mult)
				d = new Double(Tokens.get(index - 1)) * new Double(Tokens.get(index + 1));
			else
				d = new Double(Tokens.get(index - 1)) / new Double(Tokens.get(index + 1));
			Tokens.set(index - 1, d.toString());
			Tokens.remove(index);
			Tokens.remove(index);
		}
		// Low level precedence operations (+ -)
		while (true) {
			// Handling of addition and subtraction operations is similar to handling of multiplication an division. 
			int index_add = Tokens.indexOf("+");
			int index_sub = Tokens.indexOf("-");
			if (index_add < 0 && index_sub < 0)
				break;
			if (index_add < 0)
				index = index_sub;
			else if (index_sub < 0)
				index = index_add;
			else
				index = Integer.min(index_add, index_sub);
			Double d;
			if (index == index_add)
				d = new Double(Tokens.get(index - 1)) + new Double(Tokens.get(index + 1));
			else
				d = new Double(Tokens.get(index - 1)) - new Double(Tokens.get(index + 1));
			Tokens.set(index - 1, d.toString());
			Tokens.remove(index);
			Tokens.remove(index);
		}

		System.out.println(Tokens.pop());
	}
}
