/****************************************************************************
 * NCSA HDF                                                                 *
 * National Comptational Science Alliance                                   *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

/**
 * The implementation of a First-In-First-Out(FIFO) queue is based on the 
 * java.util.Stack class from SUN. Used for holding the HDF objects.
 * @author HDF group, NCSA
 */

package ncsa.hdf.util;

import java.util.Vector;

public class Queue extends Vector {

    public Queue() {
	super();
    }

    public Queue(int initialCapacity ) {
	super(initialCapacity );
    }

    /**
     * Add  all of items from the Queue into the another queue.
     * @param item the item to be put into the queue.
     */
    public Object put(Object item) {
        addElement(item);

        return item;
    }

    /**
     * Add  all of items from the Queue into the another queue.
     * @param item the item to be put into the queue.
     */
    public void Add(Queue item) {
	for (int i=item.size()-1; i>=0; i--) {
        	addElement((Object)item.elementAt(i));
	}

    }

    /**
     * Add  all of items from the Queue into the another queue.
     * @param item the item to be put into the queue.
     */
    public Queue(Queue item) {
	this();
	for (int i=item.size()-1; i>=0; i--) {
        	addElement(item.elementAt(i));
	}

    }

    /**
     * Gets an item from the front of the queue.
     * @exception EmptyQueueException If the queue is empty.
     */
    public Object get() {
        Object  obj;
        int     len = size();

        obj = peek();
        removeElementAt(0);

        return obj;
    }

    /**
     * Peeks at the front of the queue.
     * @exception EmptyQueueException If the queue is empty.
     */
    public Object peek() {
        int     len = size();

        if (len == 0)
                throw new EmptyQueueException();
        return elementAt(0);
    }

    /**
     * Returns true if the queue is empty.
     */
    public boolean empty() {
        return size() == 0;
    }

    /**
     * Sees if an object is in the queue.
     * @param obj the desired object
     * @return the distance from the front, or -1 if it is not found.
     */
    public int search(Object obj) {
        int i = indexOf(obj, 0);

        if (i >= 0) {
            return i;
        }
        return -1;
    }
}

/** 
 * Signals that the queue is empty.
 * @see Queue
 * @author HDF group, NCSA
 */

class EmptyQueueException extends RuntimeException {

    /**
     * Constructs a new EmptyQueueException with no detail message.
     * A detail message is a String that describes the exception.
     */
    public EmptyQueueException() {
      super();
    }

    /**
     * Constructs a EmptyQueueException with the specified detail message.
     * A detail message is a String that describes this particular exception.
     * @param s the detail message
     */
    public EmptyQueueException(String s) {
	super(s);
    }
}
