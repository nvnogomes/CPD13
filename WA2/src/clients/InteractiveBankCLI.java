package clients;

import java.net.UnknownHostException;
import java.util.Scanner;

import util.Deamon;
import bank.AccountManagement;
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
			if ( command.toLowerCase().equalsIgnoreCase("q") )
				break;

			BankMessage bm = BankMessage.build(command);
			bm.send();

		} while (true);

		scanIn.close();
		deamon.stopDeamon();
	}

}
