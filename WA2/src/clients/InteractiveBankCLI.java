package clients;

import java.net.UnknownHostException;
import java.util.Scanner;

import util.Deamon;
import bank.AccountManagement;
import bank.AccountManagement.BANKOP;
import bank.BankDeamon;
import bank.BankMessage;

public class InteractiveBankCLI {

	/**
	 * @param args
	 * @throws UnknownHostException
	 */
	public static void main(String[] args) throws Exception {

		Deamon deamon = new BankDeamon(AccountManagement.getInstance());
		deamon.startDeamon();

		Scanner scanIn = new Scanner(System.in);
		System.out.println("Command: OP #ACCOUNT VALUE");
		System.out.println("OP: READ WRITE");
		do {
			System.out.print("> ");

			String command = scanIn.nextLine();
			String[] split = command.split(" ");

			if (split[0].toLowerCase() == "q")
				break;

			BANKOP op = null;
			try {
				op = BANKOP.valueOf(split[0].toUpperCase());
			} catch (Exception e) {
				System.err.println("Error: Operation not recognized");
				continue;
			}
			int account = Integer.parseInt(split[1]);
			BankMessage bm = null;
			switch (op) {
			case WRITE:
				String valueStr = split[2].replaceAll("[^0-9]", ""); // bug fix
				int value = Integer.parseInt(valueStr);
				bm = BankMessage.writeMessage(account, value);
				break;
			case READ:
				bm = BankMessage.readMessage(account);
				break;
			}

			bm.send();

		} while (true);

		scanIn.close();
		deamon.stopDeamon();
	}

}
