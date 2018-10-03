package test;

public class test {
	static Counter myCounter = new Counter();

	public static void main(String[] args) throws InterruptedException {
		Runnable task = () -> {
			while(true) {
			long threadid = Thread.currentThread().getId();
			System.out.println("Hello " + threadid);
			int counter=myCounter.incrementCount();
			System.out.println("value of counter = " + counter+"  by thread id "+threadid);
			}
		};

		Thread thread1 = new Thread(task);
		Thread thread2 = new Thread(task);
		thread1.start();
		thread2.start();
		thread1.join();
		thread2.join();

		Thread.currentThread().sleep(2000);
		System.out.println("inside main thread  "+ Thread.currentThread().getId());

	}
}
