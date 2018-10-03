package test;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;



public class MyLock implements Lock {
	private int turn;
	private boolean busy = false;

	public void lock() {
		int me = (int) Thread.currentThread().getId();
		do {
			do {
				turn = me;
			} while (busy);
			busy = true;
		} while (turn != me);
	}

	public void unlock () {
	busy = false;
	}

	@Override
	public void lockInterruptibly() throws InterruptedException {
		// TODO Auto-generated method stub

	}

	@Override
	public boolean tryLock() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public boolean tryLock(long time, TimeUnit unit) throws InterruptedException {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public Condition newCondition() {
		// TODO Auto-generated method stub
		return null;
	}
}
