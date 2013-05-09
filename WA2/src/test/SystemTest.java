package test;

import java.net.SocketException;

import bank.AccountManagement;
import bank.BankDeamon;
import locker.LockDeamon;
import locker.LockServer;
import clients.BankClient;
import clients.LockClient;

/**
 * Detecção de anomalias (posição 10)
 */
public class SystemTest {

	private static final int ACCOUNT = 10;

	private static final int VALUE1 = 27;
	private static final int VALUE2 = 45;

	private static BankClient bc1;
	private static BankClient bc2;

	private static LockClient lc1;
	private static LockClient lc2;

	/**
	 * Teste 1: actualização perdida: C2 lê 10, C1 escreve 10, C2 escreve 10
	 * 
	 * @throws Exception
	 */
	public static void test1() throws Exception {
		System.out.println("Test1");
		
		lc2.lockRead(ACCOUNT);
		bc2.readOperation(ACCOUNT);
		lc2.unlockRead(ACCOUNT);

		lc1.lockWrite(ACCOUNT);
		bc1.writeOperation(ACCOUNT, VALUE1);
		lc1.unlockWrite(ACCOUNT);

		lc1.lockWrite(ACCOUNT);
		bc1.writeOperation(ACCOUNT, VALUE2);
		lc1.unlockWrite(ACCOUNT);
		
		System.out.println("");
	}

	/**
	 * Teste 2: leitura suja: C2 escreve, C1 Lê, C2 escreve
	 * 
	 * @throws Exception
	 */
	public static void test2() throws Exception {
		System.out.println("Test2");
		
		lc2.lockWrite(ACCOUNT);
		bc2.writeOperation(ACCOUNT, VALUE1);
		lc2.unlockWrite(ACCOUNT);

		lc1.lockRead(ACCOUNT);
		bc1.readOperation(ACCOUNT);
		lc1.unlockRead(ACCOUNT);

		lc1.lockWrite(ACCOUNT);
		bc1.writeOperation(ACCOUNT, VALUE2);
		lc1.unlockWrite(ACCOUNT);
		
		System.out.println("");
	}

	/**
	 * Teste 3: leitura irrepetível: C1 lê, C2 escreve, C1 lê
	 * 
	 * @throws Exception
	 */
	public static void test3() throws Exception {
		System.out.println("Test3");
		
		lc1.lockRead(ACCOUNT);
		bc1.readOperation(ACCOUNT);
		lc1.unlockRead(ACCOUNT);

		lc2.lockWrite(ACCOUNT);
		bc2.writeOperation(ACCOUNT, VALUE1);
		lc2.unlockWrite(ACCOUNT);

		lc1.lockRead(ACCOUNT);
		bc1.readOperation(ACCOUNT);
		lc1.unlockRead(ACCOUNT);
		
		System.out.println("");
	}

	/**
	 * @param args
	 * @throws SocketException
	 */
	public static void main(String[] args) throws SocketException {

		LockDeamon ld = new LockDeamon(LockServer.getInstance());
		ld.startDeamon();

		BankDeamon bd = new BankDeamon(AccountManagement.getInstance());
		bd.startDeamon();

		bc1 = new BankClient();
		bc2 = new BankClient();
		lc1 = new LockClient();
		lc2 = new LockClient();

		try {
			test1();
			test2();
			test3();
		} catch (Exception e) {
			e.printStackTrace();
		}

		ld.stopDeamon();
		bd.stopDeamon();
	}

}
