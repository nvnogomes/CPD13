package test;

import junit.framework.Assert;

import org.junit.Before;
import org.junit.Test;

import util.Status;
import bank.AccountManagement;
import bank.IBankServer;

public class BankTest {

	/**
	 * SINGLETON OBJECT 
	 * EVERY TEST USES THE SAME OBJECT CHECK THE OBJECT STATUS
	 * LEFT BY THE LAST TEST
	 */
	private IBankServer acm;

	@Before
	public void setUp() throws Exception {
		acm = AccountManagement.getInstance();
	}

	@Test
	public void testWrite() {
		Status s = acm.writeAccount(12, 30);
		Assert.assertTrue(s.getStat().ordinal() == 0);
		Assert.assertEquals(0, s.getValue());

	}

	@Test
	public void testWriteFail() {
		Status s = acm.writeAccount(-2, 30);
		Assert.assertTrue(s.getStat().ordinal() == 1);
	}

	@Test
	public void testWriteLowerLimits() {
		Status s = acm.writeAccount(0, 30);
		Assert.assertTrue(s.getStat().ordinal() == 0);
	}

	@Test
	public void testWriteUpperLimits() {
		Status s = acm.writeAccount(AccountManagement.NACCOUNTS, 30);
		Assert.assertTrue(s.getStat().ordinal() == 1);
	}

	@Test
	public void testRead() {
		acm.writeAccount(12, 30);

		Status s = acm.readAccount(12);
		Assert.assertTrue(s.getStat().ordinal() == 0);
		Assert.assertEquals(30, s.getValue());
	}

	@Test
	public void testReadFail() {
		Status s = acm.readAccount(-2);
		Assert.assertTrue(s.getStat().ordinal() == 1);
	}

	@Test
	public void testReadLowerLimits() {
		Status s = acm.readAccount(0);
		Assert.assertTrue(s.getStat().ordinal() == 0);
	}

	@Test
	public void testReadUpperLimits() {
		Status s = acm.readAccount(AccountManagement.NACCOUNTS);
		Assert.assertTrue(s.getStat().ordinal() == 1);
	}

}
