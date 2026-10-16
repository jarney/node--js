

/**
 * A qualified identifier is an identifier used for things
 * like node types, data types, graphs, etc where
 * the first portion of the name is the 'package name' or
 * 'namespace' and the second portion is the name of the item
 * within that namespace.  This is useful for looking up
 * objects across modules and stitching the modules into
 * a whole program.
 */
export class QualifiedIdentifier {
  mName: string;
  mIsQualified: boolean;
  mNamespace: string;
  constructor(aQualifiedIdentifier: string) {
    var findpos: number = aQualifiedIdentifier.lastIndexOf('.');
    if (findpos == -1) {
      this.mName = aQualifiedIdentifier;
      this.mIsQualified = false;
      this.mNamespace = "";
    }
    else {
      this.mName = aQualifiedIdentifier.substr(findpos+1);
      this.mNamespace = aQualifiedIdentifier.substr(0, findpos);
      this.mIsQualified = true;
    }
  }
  /**
   * Return the last portion of the name
   * after the last '.'.
   */
  public getName(): string {
    return this.mName;
  }
  public isQualified() : boolean {
    return this.mIsQualified;
  }
  public getNamespace() : string {
    return this.mNamespace;
  }
}

