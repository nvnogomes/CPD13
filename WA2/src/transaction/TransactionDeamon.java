package transaction;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import util.Converter;
import util.Deamon;
import util.Debug;
import util.Message;
import util.Status;

public class TransactionDeamon extends Deamon {

	/**
	 * Deamon port.
	 */
	public final static int PORT = 49999;

	/**
	 * Datagram socket.
	 */
	private DatagramSocket dSocket;

	/**
	 * 
	 */
	private ITransactionServer transServ;

	/**
	 * 
	 */
	private Debug debug;

	/**
	 * 
	 * @param ls
	 */
	public TransactionDeamon(ITransactionServer ts) {
		this(ts, new Debug(true));
	}

	/**
	 * 
	 * @param ls
	 * @param debugObj
	 */
	public TransactionDeamon(ITransactionServer ts, Debug debugObj) {
		transServ = ts;
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
		TransactionMessage tMsg = null;
		Status stat = null;
		int ret;
		try {
			dSocket.receive(packet);
			tMsg = TransactionMessage.build(Converter.byteArrayToString(buffer));
			switch (tMsg.getOp()) {
			case READ:
				ret = transServ.readT(tMsg.getAccountId());
				stat = new Status(Math.abs(ret));
				Message.sendResponse(packet.getSocketAddress(), stat.toString());
				break;
			case WRITE:
				ret = transServ.writeT(tMsg.getAccountId(), tMsg.getValue());
				stat = new Status(Math.abs(ret));
				Message.sendResponse(packet.getSocketAddress(), stat.toString());
				break;
			case COMMIT:
				ret = transServ.writeT(tMsg.getAccountId(), tMsg.getValue());
				stat = new Status(Math.abs(ret));
				Message.sendResponse(packet.getSocketAddress(), stat.toString());
				break;
			case ABORT:
				ret = transServ.writeT(tMsg.getAccountId(), tMsg.getValue());
				stat = new Status(Math.abs(ret));
				Message.sendResponse(packet.getSocketAddress(), stat.toString());
				break;
			case BEGIN:
				ret = transServ.writeT(tMsg.getAccountId(), tMsg.getValue());
				stat = new Status(Math.abs(ret));
				Message.sendResponse(packet.getSocketAddress(), stat.toString());
				break;
			default:
				stat = new Status(1);
				break;
			}

		} catch (Exception e) {
			// silence
			// e.printStackTrace();
		}
		if( tMsg != null )
		debug.show(tMsg.getOp() + "\t" + tMsg.getAccountId() + " "
				+ tMsg.getValue() + " : " + stat.getStat() + " "
				+ stat.getValue());

	}

	/**
	 * 
	 * @param args
	 */
	public static void main(String[] args) {

		TransactionDeamon td = new TransactionDeamon(
				TransactionManager.getInstance());
		try {
			td.startDeamon();
			System.out.println("TransactionDeamon running... ");
		} catch (SocketException e) {
			// silence
			// e.printStackTrace();
		}
		finally {
			td.stopDeamon();
		}
		
		
	}

}
