package test;

import locker.LockDeamon;
import locker.LockServer;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import util.Debug;

public class LockDeamonTest {

	private LockDeamon ld;
	
	
	@Before
	public void setUp() throws Exception {
		ld = new LockDeamon( LockServer.getInstance(), new Debug(false) );
		ld.startDeamon();
	}

	@After
	public void tearDown() throws Exception {
		ld.stopDeamon();
	}

	@Test
	public void testLockRead() {
		
	}

}
