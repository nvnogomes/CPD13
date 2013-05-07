package util;

import java.net.SocketException;

public abstract class Deamon extends Thread {

	/**
	 * Status of the deamon
	 */
	private boolean isActive;

	/**
	 * Starts the cycle and opens the socket
	 */
	public void startDeamon() throws SocketException {

		this.isActive = true;
		start();
	}

	/**
	 * Stop the cycle and closes the socket
	 */
	public void stopDeamon() {
		this.isActive = false;
	}

	/**
	 * Code that will be run in the thread
	 */
	public abstract void routineCycle();

	/**
	 * Thread code
	 */
	public void run() {
		while (isActive) {
			routineCycle();
		}
		System.out.println("Stopped.");
	}

}
