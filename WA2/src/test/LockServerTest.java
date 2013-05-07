package test;

import static org.junit.Assert.*;
import locker.LockServer;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import util.Status;
import util.Status.Result;

public class LockServerTest {

	/**
	 * SINGLETON OBJECT 
	 * EVERY TEST USES THE SAME OBJECT CHECK THE OBEJCT STATUS
	 * LEFT BY THE LAST TEST
	 */
	private LockServer ls;

	@Before
	public void setUp() throws Exception {
		ls = LockServer.getInstance();
	}

	@After
	public void tearDown() throws Exception {
	}

	@Test
	public void testSimpleRead() throws InterruptedException {
		Status readStat = ls.lockRead(1);
		assertTrue(readStat.getStat() == Result.OK);

		readStat = ls.unlockRead(1);
		assertTrue(readStat.getStat() == Result.OK);
	}

	@Test
	public void testSimpleWrite() throws Exception {
		Status writeStat = ls.lockWrite(1);
		assertTrue(writeStat.getStat() == Result.OK);

		writeStat = ls.unlockWrite(1);
		assertTrue(writeStat.getStat() == Result.OK);
	}

	@Test
	public void testDeadlock() throws Exception {
		Status readStat = ls.lockRead(3);
		assertTrue(readStat.getStat() == Result.OK);

		// write deadlock
		Status writeStat = ls.lockWrite(3);

		// timeout
		assertTrue(writeStat.getStat() == Result.NOT_OK);

		readStat = ls.unlockRead(3);
		assertTrue(readStat.getStat() == Result.OK);
	}

	@Test
	public void testTwoRead() throws Exception {
		Status readStat = ls.lockRead(4);
		Status twinStat = ls.lockRead(4);

		assertTrue(readStat.getStat() == Result.OK);
		assertTrue(twinStat.getStat() == Result.OK);

		readStat = ls.unlockRead(4);
		twinStat = ls.unlockRead(4);

		assertTrue(readStat.getStat() == Result.OK);
		assertTrue(twinStat.getStat() == Result.OK);

	}

	@Test
	public void testWrongOperations() throws Exception {

		Status writeStatE = ls.unlockWrite(27);
		assertTrue(writeStatE.getStat() == Result.NOT_OK);

		writeStatE = ls.unlockRead(72);
		assertTrue(writeStatE.getStat() == Result.NOT_OK);

		writeStatE = ls.unlockWrite(1024);
		assertTrue(writeStatE.getStat() == Result.NOT_OK);
	}

}
