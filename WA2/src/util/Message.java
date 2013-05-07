package util;

import java.net.*;

public class Message {

	/**
	 * Time in miliseconds to define a timeout uppon failed receiving of message
	 */
	private static final int TIMEOUT = 1000;

	/**
	 * Method that send and receives messages. This method can be accessed by
	 * the two public methods that define the wait for response or not
	 * 
	 * @param sa
	 *            SocketAddress
	 * @param msg
	 *            Message
	 * @param wait
	 *            Wait for response
	 * @return byte[]
	 */
	private static byte[] send(SocketAddress sa, String msg, boolean wait) {

		byte[] returnValue = null;
		try {
			DatagramSocket dSocket = new DatagramSocket();

			byte[] buf = msg.getBytes();
			DatagramPacket resp = new DatagramPacket(buf, buf.length, sa);
			dSocket.send(resp);

			if (wait) {
				byte[] buffer = new byte[2048];
				DatagramPacket packet = new DatagramPacket(buffer,
						buffer.length);
				dSocket.setSoTimeout(TIMEOUT);
				dSocket.receive(packet);
				returnValue = buffer;
			}

			dSocket.close();

		} catch (SocketTimeoutException e) {
		} catch (Exception e) {
			e.printStackTrace();
		}

		return returnValue;
	}

	/**
	 * This method sends a message to the given socket and waits for a response
	 * 
	 * @param sa
	 *            SocketAddress
	 * @param rsp
	 *            Message
	 * @return byte[]
	 */
	public static byte[] sendOperation(SocketAddress sa, String rsp) {
		return send(sa, rsp, true);
	}

	/**
	 * This method sends a message without waiting for a response
	 * 
	 * @param sa
	 *            SocketAddress
	 * @param rsp
	 *            Message
	 */
	public static void sendResponse(SocketAddress sa, String rsp) {
		send(sa, rsp, false);
	}

}