/**
 * A DataType in NodeJS represents the type of data
 * flowing along a connection between nodes.  The physical
 * storage of data types is abstract and only identified by a
 * name.  The implementation of the physical means of connection
 * is governed by the specific nodes and the implementing platform.
 */
class DataType {
    mId: string;
    mName: string;
    /**
     * Cretes a new data type.
     * @param id Unique identifier
     * @param name Name which is human-readable.
     */
    public constructor(aId: string, aName: string) {
        this.mId = aId;
	this.mName = aName;
    }

    /**
     * Returns the unique identifier for this data type.
     */
    public getId(): string {
        return this.mId;
    }

    /**
     * Returns the human-readable name for this data type.
     */
    public getName() : string {
        return this.mName;
    }

    /**
     * Two data types compare as equal if and only if
     * their unique IDs are the same.  Note that their
     * names are permitted to differ and still be
     * considered the same.
     */
     public static equals(a: DataType, b: DataType) : boolean {
        return a.mId == b.mId;
    }
}

export { DataType };
