package clients;

import java.net.UnknownHostException;
import java.util.Scanner;

import locker.LockDeamon;
import locker.LockMessage;
import locker.LockServer;
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
			if (command.toLowerCase().equalsIgnoreCase("q"))
				break;

			LockMessage bm = LockMessage.build(command);
			bm.send();

		} while (true);

		scanIn.close();
		deamon.stopDeamon();
	}

}
