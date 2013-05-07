package test;

import junit.framework.Assert;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import util.Deamon;
import util.Status;
import util.Status.Result;
import bank.AccountManagement;
import bank.BankDeamon;
import bank.BankMessage;

public class BankDeamonTest {

	private Deamon bdm;
	
	@Before
	public void setUp() throws Exception {
		bdm = new BankDeamon( AccountManagement.getInstance() );
		bdm.startDeamon();
	}
	
	@Test
	public void testSimpleWrite() {
		BankMessage bm = BankMessage.writeMessage(1, 20);
		
		try {
			Status stat = bm.send();
			
			Assert.assertTrue(stat.getStat() == Result.OK);
		} catch (Exception e) {
			e.printStackTrace();
			Assert.fail("Address error");
		}
	}
	
	@Test
	public void testSimpleRead() {
		BankMessage bm = BankMessage.readMessage(1);
		
		try {
			Status stat = bm.send();
			
			Assert.assertTrue(stat.getStat() == Result.OK);
			Assert.assertTrue(stat.getValue() == 20);
		} catch (Exception e) {
			Assert.fail("Address error");
		}		
	}
	
	@Test
	public void testSimpleReadError() {
		BankMessage bm = BankMessage.readMessage(1024);
		
		try {
			Status stat = bm.send();
			Assert.assertTrue(stat.getStat() == Result.NOT_OK);
			Assert.assertTrue(stat.getValue() == 0);
		} catch (Exception e) {
			Assert.fail("Address error");
		}
	}
	
	@After
	public void tearDown() {
		bdm.stopDeamon();
	}

}
