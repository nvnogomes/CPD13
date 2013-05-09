package locker;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import util.Converter;
import util.Deamon;
import util.Debug;
import util.Message;
import util.Status;

public class LockDeamon extends Deamon {

	/**
	 * Deamon port.
	 */
	public final static int PORT = 46666;

	/**
	 * Datagram socket.
	 */
	private DatagramSocket dSocket;

	/**
	 * 
	 */
	private Debug debug;

	/**
	 * 
	 */
	private ILockServer locker;

	/**
	 * 
	 * @param ls
	 */
	public LockDeamon(ILockServer ls) {
		this(ls, new Debug(true));
	}

	/**
	 * 
	 * @param ls
	 * @param debugObj
	 */
	public LockDeamon(ILockServer ls, Debug debugObj) {
		locker = ls;
		debug = debugObj;
	}

	@Override
	public void startDeamon() throws SocketException {
		super.startDeamon();
		dSocket = new DatagramSocket(PORT);
	}

	@Override
	public void stopDeamon() {
		super.stopDeamon();
		dSocket.close();
	}

	@Override
	public void routineCycle() {
		byte[] buffer = new byte[2048];
		DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
		LockMessage lMsg = null;
		Status ret = null;
		try {
			dSocket.receive(packet);
			lMsg = LockMessage.build(Converter.byteArrayToString(buffer));
			switch (lMsg.getOp()) {
			case LREAD:
				ret = locker.lockRead(lMsg.getAccountId());
				Message.sendResponse(packet.getSocketAddress(), ret.toString());
				break;
			case LWRITE:
				ret = locker.lockWrite(lMsg.getAccountId());
				Message.sendResponse(packet.getSocketAddress(), ret.toString());
				break;
			case UREAD:
				ret = locker.unlockRead(lMsg.getAccountId());
				Message.sendResponse(packet.getSocketAddress(), ret.toString());
				break;
			case UWRITE:
				ret = locker.unlockWrite(lMsg.getAccountId());
				Message.sendResponse(packet.getSocketAddress(), ret.toString());
				break;
			default:
				ret = new Status(1);
				break;
			}
		} catch (Exception e) {
			// silence
			// e.printStackTrace();
		}
		if( lMsg != null && ret != null )
			debug.show(lMsg.getOp() + "\t" + lMsg.getAccountId() + " : "
				+ ret.getStat());
	}

	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {

		LockDeamon ld = new LockDeamon(LockServer.getInstance());
		try {
			ld.startDeamon();
			System.out.println("LockDeamon running... ");
		} catch (SocketException e) {
			// silence
			// e.printStackTrace();
		}
		finally {
			ld.stopDeamon();
		}
	}

}
