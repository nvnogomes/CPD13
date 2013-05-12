package test;

import static org.junit.Assert.assertTrue;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import transaction.TransactionManager;

public class TransactionServerTest {

	private TransactionManager tm;
	private int trId;
	
	@Before
	public void setUp() throws Exception {
		tm = TransactionManager.getInstance();
		if( trId > 0 ) {
			tm.commitT(trId); // clear all locks
		}
		tm.reset();
	}

	@After
	public void tearDown() throws Exception {

	}

	@Test
	public void testBeginT() {
		trId = tm.beginT();
		System.out.println("New transaction, id: "+trId);
		assertTrue("TransactionId > 0", trId > 0 );
	}
	
	@Test
	public void testReadT() {
		trId = tm.beginT();
		int result = tm.readT(10);
		assertTrue("ReadT ",result == 0);
	}
	
	@Test
	public void testWriteT() {
		tm.commitT(trId); // clear locks
		trId = tm.beginT();
		int result = tm.writeT(10,10);
		assertTrue("WriteT ",result == 0);
	}
	
	@Test
	public void testAbortT() {
		trId = tm.beginT();
		int result = tm.abortT(trId);
		assertTrue("AbortT ",result == 0);
	}
	
	@Test
	public void testCommitT() {
		trId = tm.beginT();
		int result = tm.commitT(trId);
		assertTrue("CommitT ",result == 0);
	}

}
