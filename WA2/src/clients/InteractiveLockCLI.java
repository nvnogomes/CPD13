package clients;

import java.net.UnknownHostException;
import java.util.Scanner;

import locker.LockDeamon;
import locker.LockMessage;
import locker.LockServer;
import locker.LockServer.LOCKOP;
import util.Deamon;
import util.Status;

public class InteractiveLockCLI {

	/**
	 * @param args
	 * @throws UnknownHostException
	 */
	public static void main(String[] args) throws Exception {

		Deamon deamon = new LockDeamon(LockServer.getInstance());
		deamon.startDeamon();

		Scanner scanIn = new Scanner(System.in);
		do {
			System.out.println("Available operations:");
			System.out.println("LREAD UREAD LWRITE UWRITE");
			System.out.print("> ");

			String command = scanIn.nextLine();
			String[] split = command.split(" ");

			if (split[0].toLowerCase() == "q")
				break;

			LOCKOP op = LOCKOP.valueOf(split[0].toUpperCase());
			int account = Integer.parseInt(split[1]);
			LockMessage lm = new LockMessage(op, account);

			Status stat = lm.send();
			System.out.println(stat.toString());

		} while (true);

		scanIn.close();
		deamon.stopDeamon();
	}

}
