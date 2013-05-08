package clients;

import java.net.UnknownHostException;
import java.util.Scanner;

import locker.LockDeamon;
import locker.LockMessage;
import locker.LockServer;
import locker.LockServer.LOCKOP;
import util.Deamon;

public class InteractiveLockCLI {

	/**
	 * @param args
	 * @throws UnknownHostException
	 */
	public static void main(String[] args) throws Exception {

		Deamon deamon = new LockDeamon(LockServer.getInstance());
		deamon.startDeamon();

		Scanner scanIn = new Scanner(System.in);
		System.out.println("Command: OP #ACCOUNT");
		System.out.println("OP: LREAD UREAD LWRITE UWRITE");
		do {

			System.out.print("> ");

			String command = scanIn.nextLine();
			String[] split = command.split(" ");

			if (split[0].toLowerCase() == "q")
				break;
			
			LOCKOP op = null;
			try {
				op = LOCKOP.valueOf(split[0].toUpperCase());
			} catch (Exception e) {
				System.err.println("Error: Operation not recognized");
				continue;
			}
			int account = Integer.parseInt(split[1]);
			LockMessage lm = new LockMessage(op, account);

			lm.send();

		} while (true);

		scanIn.close();
		deamon.stopDeamon();
	}

}
