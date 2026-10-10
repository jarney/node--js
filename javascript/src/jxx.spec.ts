import { describe, it, expect } from 'vitest'
import { mount } from '@vue/test-utils'
import {DataType} from './DataType.ts'

describe('DataType', () => {
  it('test_DataType_constructor', () => {
	var dataType1: DataType = new DataType("int", "Integer");
	var dataType2: DataType = new DataType("int", "Another integer");
	
	expect(DataType.equals(dataType1, dataType2));
	
	// Things may compare equal as long as the ID is the same.
	// even if they have different names.
	expect(dataType1.getName() != dataType2.getName());
  })
})
