package test;

import org.junit.Before;
import org.junit.Test;

import bank.AccountManagement;
import bank.BankDeamon;

public class BankDeamonTest {

	private BankDeamon bdm;
	
	@Before
	public void setUp() throws Exception {
		bdm = new BankDeamon( AccountManagement.getInstance() );
	}
	
	@Test
	public void testSimpleWrite() {
	}
	
	@Test
	public void testSimpleRead() {
		
	}

}
