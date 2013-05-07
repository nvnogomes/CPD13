package clients;

import java.net.UnknownHostException;
import java.util.Scanner;

import util.Deamon;
import util.Status;
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
	
		Deamon deamon = new BankDeamon( AccountManagement.getInstance() );
		deamon.startDeamon();
		
		
		Scanner scanIn = new Scanner(System.in);
		do {
			System.out.print("> ");
			
			
			String command = scanIn.nextLine();
			String[] split = command.split(" ");
			
			if( split[0].toLowerCase() == "q") break;
			
			BANKOP op = BANKOP.valueOf(split[0]);
			int account = Integer.parseInt(split[1]);
			BankMessage bm = null;
			switch(op) {
			case WRITE:
				String valueStr = split[2].replaceAll("[^0-9]",""); // bug fix
				int value = Integer.parseInt(valueStr);
				bm = BankMessage.writeMessage(account, value);
				break;
			case READ:
				bm = BankMessage.readMessage(account);
				break;
			}
			
			Status stat = bm.send();
			System.out.println( stat.toString() );
			
		} while (true);

		scanIn.close();
		deamon.stopDeamon();
	}

}
