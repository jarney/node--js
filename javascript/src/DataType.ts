import type {DataTypeId} from "./Definitions.ts"

/**
 * A DataType in NodeJS represents the type of data
 * flowing along a connection between nodes.  The physical
 * storage of data types is abstract and only identified by a
 * name.  The implementation of the physical means of connection
 * is governed by the specific nodes and the implementing platform.
 */
export class DataType {
  mId: DataTypeId;
  mFullyQualifiedId: string;
  mName: string;
    /**
     * Cretes a new data type.
     * @param id Unique identifier
     * @param name Name which is human-readable.
     */
  public constructor(aId: DataTypeId, aPackage: string, aName: string) {
    this.mId = aId;
    this.mFullyQualifiedId = aPackage + "." + aId;
    this.mName = aName;
  }

  public copy(other: DataType) {
    this.mId = other.mId;
    this.mFullyQualifiedId = other.mFullyQualifiedId;
    this.mName = other.mName;
  }
    

  /**
   * Returns the unique identifier for this data type.
   */
  public getLocalId(): DataTypeId {
    return this.mId;
  }
  public getFullyQualifiedId(): DataTypeId {
    return this.mFullyQualifiedId;
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
    return a.mFullyQualifiedId == b.mFullyQualifiedId;
  }
}

