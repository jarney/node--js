import {DataType} from './DataType.ts'
import {NodeType} from './NodeType.ts'


class NodeModule {
      mPackage: string = "";
      mDataTypes: { [key: string]: DataType } = {};
      mNodeTypes: { [key: string]: NodeType } = {};

      constructor() {
          this.mPackage = "";
	  this.mDataTypes = {};
	  this.mNodeTypes = {};
      }
      public setPackage(aPackage: string) {
          this.mPackage = aPackage;
      }
      public getPackage() : string {
          return this.mPackage;
      }
      public addDataType(aDataType: DataType) {
          if (aDataType.getId() in this.mDataTypes) return;
          this.mDataTypes[aDataType.getId()] = aDataType;
      }
      public removeDataType(aId: string) {
          delete this.mDataTypes[aId];
      }
/*
    def getDataTypes(self):
        return self.dataTypes

    def hasDataType(self, id):
        return (id in self.dataTypes)

    def getDataType(self, id):
        return self.dataTypes[id]

    def addNodeType(self, nodeType: NodeType):
        self.nodeTypes[nodeType.getId()] = nodeType

    def getNodeTypes(self):
        return self.nodeTypes;

    def hasNodeType(self, id):
        return (id in self.nodeTypes)

    def getNodeType(self, id):
        return self.nodeTypes[id]
    
*/

}



