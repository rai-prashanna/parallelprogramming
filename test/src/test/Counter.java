package test;

import java.util.concurrent.locks.Lock;

public class Counter {
	private int count = 0;
	private Lock mylock = new MyLock();

	public int incrementCount() {
		try {
			mylock.lock();
			count=count+1;
			mylock.unlock();
		
		} 
		catch (Exception e) {
			System.out.println("exception has been caught  "+ e);
		}
		finally {
			mylock.unlock();
		}
		return count;
	}
}
